FROM python:3.9-slim

# Install C++ build tools, flex, and bison
RUN apt-get update && apt-get install -y \
    build-essential \
    flex \
    bison \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy and install Python dependencies
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy all project files
COPY . .

# Build the compiler (This creates the './compiler' Linux executable)
RUN make

# Ensure the compiler is executable
RUN chmod +x ./compiler

EXPOSE 10000

CMD ["python", "app.py"]