#syntax=docker/dockerfile:1

# # Comments are provided throughout this file to help you get started.
# # If you need more help, visit the Dockerfile reference guide at
# # https://docs.docker.com/go/dockerfile-reference/

# # Want to help us make this template better? Share your feedback here: https://forms.gle/ybq9Krt8jtBL3iCk7

# ARG PYTHON_VERSION=3.12.3
# FROM python:${PYTHON_VERSION} as base

# # Prevents Python from writing pyc files.
# ENV PYTHONDONTWRITEBYTECODE=1

# # Keeps Python from buffering stdout and stderr to avoid situations where
# # the application crashes without emitting any logs due to buffering.
# ENV PYTHONUNBUFFERED=1

# WORKDIR /app

# # Create a non-privileged user that the app will run under.
# # See https://docs.docker.com/go/dockerfile-user-best-practices/
# # ARG UID=10001
# # RUN adduser \
# #     --disabled-password \
# #     --gecos "" \
# #     --home "/nonexistent" \
# #     --shell "/sbin/nologin" \
# #     --no-create-home \
# #     --uid "${UID}" \
# #     appuser

# RUN curl https://sh.rustup.rs -sSf | bash -s -- -y
# ENV PATH="/root/.cargo/bin:${PATH}"
# # Copy the source code into the container.
# COPY app.py .
# # Download dependencies as a separate step to take advantage of Docker's caching.
# # Leverage a cache mount to /root/.cache/pip to speed up subsequent builds.
# # Leverage a bind mount to requirements.txt to avoid having to copy them into
# # into this layer.
# RUN --mount=type=cache,target=/root/.cache/pip \
#     --mount=type=bind,source=requirements.txt,target=requirements.txt \
#     python -m pip install -r requirements.txt

# RUN pip install uvicorn
# # Switch to the non-privileged user to run the application.
# # USER appuser



# # Expose the port that the application listens on.
# EXPOSE 8765

# # Run the application.
# CMD ["python", "./app.py"]
# #CMD ["uvicorn", "app:app", "--host", "0.0.0.0", "--port", "8765"]

# Use the official Python image
FROM python:3.12.3

# Set the working directory in the container
WORKDIR /app

# Copy the requirements file into the container
COPY requirements.txt .

# Install the dependencies
RUN pip install --no-cache-dir -r requirements.txt

# Copy the FastAPI app code into the container
COPY main.py .

# Expose the port that the FastAPI server runs on
EXPOSE 8000

# Command to run the FastAPI server
CMD ["uvicorn", "main:app_sio", "--host", "0.0.0.0", "--port", "8000"]
