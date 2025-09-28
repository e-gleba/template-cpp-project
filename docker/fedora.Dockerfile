FROM fedora:latest

WORKDIR /app

# Install essential C++ development tools, documentation, and workflow tools
RUN dnf -y upgrade --refresh && \
    dnf -y install \
    clang \
    clang-tools-extra \
    doxygen \
    cmake \
    git \
    ninja-build \
    make \
    gcc-c++ \
    && dnf clean all

COPY . /app

# Validate CMake preset early (fail fast if misconfigured)
RUN cmake --preset=gcc-full --check-presets

# Default entrypoint: run the full build/test workflow
ENTRYPOINT ["cmake", "--workflow", "--preset=gcc-full"]

# Fallback: interactive shell if needed (e.g., docker run -it --entrypoint /bin/bash ...)
# CMD ["/bin/bash"]
