from flask import Flask, request, jsonify, render_template
import subprocess
import os

app = Flask(__name__)

# 1. Serve the HTML web page
@app.route('/')
def home():
    return render_template('index.html')

# 2. Handle the compilation request from the web page
@app.route('/compile', methods=['POST'])
def compile_code():
    code = request.json.get('code', '')
    
    # Write the user's code to the test file
    with open('test/test.c', 'w', encoding='utf-8') as f:
        f.write(code)
        
    # Run your compiler executable
    try:
        # Use 'compiler.exe' for Windows
        result = subprocess.run(['./compiler', 'test/test.c'], capture_output=True, text=True)
    except Exception as e:
        return jsonify({'error': str(e), 'assembly': '', 'console': 'Failed to run compiler.exe'}), 500
        
    # Read the generated assembly file
    asm_output = ""
    if os.path.exists('output/output.asm'):
        with open('output/output.asm', 'r', encoding='utf-8') as f:
            asm_output = f.read()
            
    # Combine stdout and stderr for the console view
    console_output = result.stdout + result.stderr
            
    return jsonify({
        'assembly': asm_output,
        'console': console_output
    })

if __name__ == '__main__':
    app.run(debug=True)