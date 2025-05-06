FROM archlinux:latest

# Update and install base dependencies, sudo, git, and github-cli
# Also install 'which' as it's often used by scripts and might be missing
RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm bash sudo git github-cli which && \
    # Clean up pacman cache
    pacman -Scc --noconfirm

# Create the codespace user with a home directory, add to wheel group (for sudo)
# Set bash as the default shell
RUN useradd -m -G wheel -s /bin/bash codespace && \
    # Setup passwordless sudo for the codespace user
    echo 'codespace ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/codespace && \
    chmod 0440 /etc/sudoers.d/codespace

# Switch to the codespace user for subsequent commands if needed (though not strictly necessary here
# as further commands will be run via devcontainer.json lifecycle scripts)
# USER codespace
# ENV HOME=/home/codespace

# Set a default command if the container is run directly (optional)
# CMD ["/bin/bash"]