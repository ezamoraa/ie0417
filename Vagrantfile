# -*- mode: ruby -*-
# vi: set ft=ruby :

# (C) Copyright 2020-2022 Hewlett Packard Enterprise Development LP.

Vagrant.require_version ">= 1.8.1"
ENV['VAGRANT_DEFAULT_PROVIDER'] = 'docker'

if !Vagrant.has_plugin?("vagrant-proxyconf") && ENV.has_value?("http_proxy")
    system('vagrant plugin install vagrant-proxyconf')
    raise("vagrant-proxyconf installed. Run command again.")
end

if ! ENV.has_key?('SSH_AUTH_SOCK')
    raise("SSH_AUTH_SOCK not found, please setup your ssh-agent")
end

version = File.read("tools/docker/version").strip
registry = "docker.io/"
image_name = registry + "ezamoraa/ie0317:" + version

$adjust_user_script = <<SCRIPT
sed -i -e "s/1000/$HOST_UID/" /etc/passwd
sed -i -e "s/1000/$HOST_GID/" /etc/passwd
sed -i -e "s/1000/$HOST_GID/" /etc/group
chown -R $HOST_UID:$HOST_GID /home/dev/ 2>/dev/null 1>&2 || true
SCRIPT

Vagrant.configure("2") do |config|
  config.vm.hostname = "ie0317-devel"
  config.vm.provider "docker" do |docker|
    docker.image = image_name
    docker.has_ssh = true
    docker.auth_server = registry
    docker.pull = true
  end
  config.ssh.username = "dev"
  config.ssh.forward_agent = true
  config.ssh.pty = true
  config.ssh.forward_x11 = true
  config.vm.synced_folder ".", "/home/dev/ws"
  # Used when running the live documentation server
  config.vm.network "forwarded_port", guest: 8000, host: 8000
  config.vm.network "forwarded_port", guest: 8888, host: 8888
  if File.exist?("/tmp/.X11-unix")
    config.vm.synced_folder "/tmp/.X11-unix", "/tmp/.X11-unix"
  end
  if File.exist?(ENV['HOME'] + "/.vscode/")
    config.vm.synced_folder ENV['HOME'] + "/.vscode/", "/home/dev/.vscode/"
  end

  if File.directory?(ENV["HOME"] + "/.Xauthority")
    config.vm.synced_folder ENV["HOME"] + "/.Xauthority", "/home/dev/.Xauthority", create: true
  end
  config.vm.synced_folder ENV["HOME"] + "/.config", "/home/dev/.config", create: false
  config.vm.provision "file",
    source: "~/.gitconfig",
    destination: "/home/dev/.gitconfig" if File.exist?(ENV["HOME"] + "/.gitconfig")
  config.vm.provision "file",
    source: "~/.gitignore",
    destination: "/home/dev/.gitignore" if File.exist?(ENV["HOME"] + "/.gitignore")
  config.vm.provision "file",
    source: "~/.gitalias.txt",
    destination: "/home/dev/.gitalias.txt" if File.exist?(ENV["HOME"] + "/.gitalias.txt")
  config.vm.provision "shell", inline: 'echo "cd /home/dev/ws/" > /etc/profile.d/ws.sh', run: "always"
  config.vm.provision "shell", inline: $adjust_user_script,
    env: {
      "HOST_UID" => Process.uid,
      "HOST_GID" => Process.gid
    }
  if Vagrant.has_plugin?("vagrant-proxyconf")
    if ENV.has_value?("http_proxy")
      config.proxy.http = ENV["http_proxy"]
      config.proxy.https = ENV["https_proxy"]
      config.proxy.no_proxy = ENV["no_proxy"]
    end
  end
end
