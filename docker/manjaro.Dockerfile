# Use a lightweight base image
FROM manjarolinux/base:latest

# Set the working directory
WORKDIR /app

# Install essential C++ development tools, documentation, and workflow tools
RUN pacman -Syu --noconfirm && \
    pacman -S --needed --noconfirm \
    base-devel \
    clang \
    clang-doc \
    doxygen \
    cmake \
    git \
    ninja \
    && pacman -Scc --noconfirm

# Copy the source code into the container
COPY . /app

# Validate CMake preset early to fail fast on configuration errors
RUN cmake --preset=gcc-full --check-presets

# Example: Run a specific CMake workflow preset
ENTRYPOINT ["cmake", "--workflow", "--preset=gcc-full"]

# Or, keep an interactive shell for development
# CMD ["/bin/bash"]
