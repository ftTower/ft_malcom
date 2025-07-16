
## Virtual Machine Configuration

Two VMs are used: one for the victim, one for Malcolm.

- **Debian 12.11.0 Image**: [Download](https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/debian-12.11.0-amd64-netinst.iso)

### Root Login

```bash
su -
```

### Install Required Packages (on both VMs)

```bash
sudo apt-get update && sudo apt-get upgrade && sudo apt-get install -y vim git net-tools iproute2 arping iputils-ping tcpdump
git clone https://github.com/ftTower/ft_malcom.git ft_malcolm && cd ft_malcolm && clear && echo done 
```

---

### Victim VM Network Configuration

```bash
sudo bash -c 'echo -e "# This file describes the network interfaces available on your system\n# and how to activate them. For more information see interfaces(5).\n\nsource /etc/network/interfaces.d/*\n\n# The loopback network interface\nauto lo\niface lo inet loopback\n\n# The primary network interface\nauto enp0s3\niface enp0s3 inet static\n    address 192.168.1.20\n    netmask 255.255.255.0" > /etc/network/interfaces'
echo "Network configuration replaced. Implementing NAT network."
sudo systemctl restart networking
clear && ip a
```

---

### Malcolm VM Network Configuration

```bash
sudo bash -c 'echo -e "# This file describes the network interfaces available on your system\n# and how to activate them. For more information see interfaces(5).\n\nsource /etc/network/interfaces.d/*\n\n# The loopback network interface\nauto lo\niface lo inet loopback\n\n# The primary network interface\nauto enp0s3\niface enp0s3 inet static\n    address 192.168.1.10\n    netmask 255.255.255.0" > /etc/network/interfaces'
echo "Network configuration replaced. Implementing NAT network."
sudo systemctl restart networking
clear && ip a
```

---

### Expected Result (`ip a`)

- **Victim**: `inet 192.168.1.20`

![Screenshot of Malcolm VM network configuration](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_a_output.png)

- **Malcolm**: `inet 192.168.1.10`

