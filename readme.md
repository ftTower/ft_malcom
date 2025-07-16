# FT_MALCOLM

---


## Virtual Machine Setup

This project uses two virtual machines: one as the victim and one as Malcolm (the attacker).

- **Debian 12.11.0 Image**: [Download](https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/debian-12.11.0-amd64-netinst.iso)

**Virtualization:** Oracle VM VirtualBox is recommended.

**Tip:** Install Guest Additions for clipboard sharing and drag-and-drop.

### Shared Clipboard Configuration

After creating both VMs, enable clipboard sharing:

- In VirtualBox, go to **Devices > Shared Clipboard > Bidirectional** for each VM.

### Root Access

To avoid modifying the sudoers file, switch to root:

```bash
su -
```

### Installing Dependencies (on Malcolm VM)

Update and install required packages:

```bash
sudo apt-get update && sudo apt-get upgrade
sudo apt-get install -y vim git net-tools iproute2 arping iputils-ping tcpdump
git clone https://github.com/ftTower/ft_malcom.git ft_malcolm
cd ft_malcolm
echo done
```

---

## VM Network Configuration

Configure both VMs to use DHCP for the NAT network:

```bash
sudo bash -c 'cat <<EOF > /etc/network/interfaces
# This file describes the network interfaces available on your system
# and how to activate them. For more information see interfaces(5).

source /etc/network/interfaces.d/*

# The loopback network interface
auto lo
iface lo inet loopback

# The primary network interface
auto enp0s3
iface enp0s3 inet dhcp
EOF'
echo "Network configuration replaced. Implementing DHCP for NAT network."
sudo systemctl restart networking
clear && ip a
```

### Creating a NAT Network

1. In VirtualBox, go to **File > Tools > Network Manager > NAT Networks** and create a new NAT network.
    ![Screenshot of Vbox](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/Vbox_NAT_network.png)

2. For both VMs, go to **Machine > Settings > Network**, set "Attached to" as **NAT Network**, and select your created network.
    ![Screenshot of VM](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/vm_network.png)

---

## Man-in-the-Middle Attack

### Usage

After configuring the VMs:

- **(Optional)** Use `nmap` to scan IPs in a real scenario.

#### Gather Network Information

On Malcolm VM:

```bash
ip -r
```
- Output: `default via <gateway_ip>`

![Screenshot of Malcolm](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_r_output_malcolm.png)

```bash
ip -a
```
- Output: `link/ether <Malcolm_MAC_Address>`

![Screenshot of Malcolm](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_a_output_malcolm.png)

On Victim VM:

```bash
ip -a
```
- Output: `inet <Victim_IP>`
- Output: `link/ether <Victim_MAC_Address>`

![Screenshot of Victim](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_a_output_victim.png)

#### Run FT_MALCOLM

```bash
./ft_malcolm <gateway_ip> <malcolm_mac_address> <victim_ip> <victim_mac_address>
```

- `<gateway_ip>`: Found with `ip -r` on Malcolm VM.
- `<malcolm_mac_address>`: Found with `ip -a` on Malcolm VM.
- `<victim_ip>` and `<victim_mac_address>`: Found with `ip -a` on Victim VM.

---
