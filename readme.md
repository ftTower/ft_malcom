# FT_MALCOLM

## Virtual Machine Configuration

This setup uses two virtual machines: one for the victim and one for Malcolm.

- **Debian 12.11.0 Image**: [Download](https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/debian-12.11.0-amd64-netinst.iso)

For this demonstration, Oracle VM VirtualBox is used.

**Remember to install Guest Additions to enable copy/paste functionality.**

After creating both machines:

In VirtualBox, for both VMs:  
Go to **Devices > Shared Clipboard > Bidirectional**

### Root Login (to avoid adding yourself to the sudoers file)

```bash
su -
```

### Install Required Packages (on Malcolm vm)

```bash
sudo apt-get update && sudo apt-get upgrade && sudo apt-get install -y vim git net-tools iproute2 arping iputils-ping tcpdump
git clone https://github.com/ftTower/ft_malcom.git ft_malcolm && cd ft_malcolm && clear && echo done 
```

---
### VM Network Configuration (Victim & Malcolm)

For both the Victim and Malcolm VMs, configure the network interface to use DHCP for the NAT network:

```bash
sudo bash -c 'echo -e "# This file describes the network interfaces available on your system\n# and how to activate them. For more information see interfaces(5).\n\nsource /etc/network/interfaces.d/*\n\n# The loopback network interface\nauto lo\niface lo inet loopback\n\n# The primary network interface\nauto enp0s3\niface enp0s3 inet dhcp" > /etc/network/interfaces'
echo "Network configuration replaced. Implementing DHCP for NAT network."
sudo systemctl restart networking
clear && ip a
```

---

### Expected Result (`ip a`)

- **Malcolm VM**: `inet 192.168.1.10`
![Screenshot of Malcolm VM network configuration](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_a_output_malcolm.png)

- **Victim VM**: `inet 192.168.1.20`
![Screenshot of Malcolm VM network configuration](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_a_output_victim.png)

### Make a NAT network

- In Oracle VM VirtualBox, go to **File > Tools > Network Manager > NAT Networks** and create a network with a name of your choice.
![Screenshot of Vbox](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/Vbox_NAT_network.png)

- For both VMs, go to **Machine > Settings > Network**, change "Attached to" from "NAT" to "NAT Network", and select the network you created.
![Screenshot of VM](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/vm_network.png)

