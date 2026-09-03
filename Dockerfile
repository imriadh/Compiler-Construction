# Use a lightweight Python image
FROM python:3.9-slim

# Install C++ compiler, make, flex, and bison
RUN apt-get update && apt-get install -y \
    build-essential \
    flex \
    bison \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy Python requirements and install them
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy all your project files to the server
COPY . .

# Build the compiler on the Linux server
RUN make

# Expose the port Render expects
EXPOSE 10000

# Start the Flask app
CMD ["python", "app.py"]