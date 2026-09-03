from flask import Flask, request, jsonify, render_template
import subprocess
import os

app = Flask(__name__)

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/compile', methods=['POST'])
def compile_code():
    code = request.json.get('code', '')
    
    # Ensure directories exist
    os.makedirs('test', exist_ok=True)
    os.makedirs('output', exist_ok=True)
    
    # Write user code to file
    with open('test/test.c', 'w', encoding='utf-8') as f:
        f.write(code)
        
    try:
        # On Render (Linux), the file is named 'compiler', NOT 'compiler.exe'
        compiler_path = './compiler'
        
        if not os.path.exists(compiler_path):
            return jsonify({
                'assembly': '',
                'console': f"ERROR: '{compiler_path}' not found!\n\nThis means the 'make' command failed during deployment. Please check the Render 'Build Logs' tab for C++ compilation errors."
            }), 500
            
        # Run the compiler
        result = subprocess.run([compiler_path, 'test/test.c'], capture_output=True, text=True, timeout=10)
        
    except Exception as e:
        return jsonify({
            'assembly': '',
            'console': f"CRASH: {str(e)}"
        }), 500
        
    # Read the generated assembly
    asm_output = ""
    if os.path.exists('output/output.asm'):
        with open('output/output.asm', 'r', encoding='utf-8') as f:
            asm_output = f.read()
            
    console_output = result.stdout + result.stderr
            
    return jsonify({
        'assembly': asm_output,
        'console': console_output
    })

if __name__ == '__main__':
    # Render requires host='0.0.0.0' and port 10000 (or $PORT)
    port = int(os.environ.get('PORT', 10000))
    app.run(host='0.0.0.0', port=port, debug=False)