# FT_MALCOLM

---

## Overview

**FT_MALCOLM** is a project that demonstrates a Man-in-the-Middle (MITM) attack using ARP spoofing in a controlled virtual environment. This guide walks you through setting up two Debian virtual machines (one attacker, one victim), configuring their networks, and running the FT_MALCOLM tool to observe ARP poisoning in action. The document also explains the underlying network concepts and protocols involved.

---

## Virtual Machine Setup

This project requires two virtual machines: one as the victim and one as Malcolm (the attacker).

- **Debian 12.11.0 Image:** [Download](https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/debian-12.11.0-amd64-netinst.iso)
- **Virtualization:** Oracle VM VirtualBox is recommended.

**Tip:** Install Guest Additions for clipboard sharing and drag-and-drop functionality.

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

![Malcolm ip -r output](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_r_output_malcolm.png)

```bash
ip -a
```
- Output: `link/ether <Malcolm_MAC_Address>`

![Malcolm ip -a output](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_a_output_malcolm.png)

On Victim VM:

```bash
ip -a
```
- Output: `inet <Victim_IP>`
- Output: `link/ether <Victim_MAC_Address>`

![Victim ip -a output](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_a_output_victim.png)

#### Running FT_MALCOLM

On the Malcolm VM, start the attack tool with:

```bash
./ft_malcolm <gateway_ip> <malcolm_mac_address> <victim_ip> <victim_mac_address>
```
![Malcolm inputs](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/malcolm_inputs.png)

- `<gateway_ip>`: Obtain with `ip -r` on Malcolm VM.
- `<malcolm_mac_address>`: Obtain with `ip -a` on Malcolm VM.
- `<victim_ip>` and `<victim_mac_address>`: Obtain with `ip -a` on Victim VM.

Once FT_MALCOLM is running and listening, execute the following on the Victim VM to flush the ARP cache and trigger a new ARP request:

![Malcolm waiting](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/malcolm_waiting.png)
```bash
ip -s -s neigh flush all && ping -c 1 <gateway_ip>
```

![Malcolm injected](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/malcolm_injected.png)
Malcolm should detect the ARP request from the Victim VM to the gateway and respond accordingly.

To verify if the victim is ARP poisoned, display the ARP table on the Victim VM:

```bash
ip neigh show
```
![Victim ARP table](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/arp_table.png)

You should see the gateway IP associated with Malcolm's MAC address, indicating a successful ARP spoofing attack.


You can observe the traffic with this command:
```bash
tcpdump -vv -i <interface> arp
```
<interface> on 42 computer : enp0s3

---

## How It Works

### Typical Ethernet Frame Content

- **Preamble and Start Frame Delimiter (SFD):** Used for clock synchronization between devices.
- **Destination MAC Address:** The physical address of the recipient's network card.
- **Source MAC Address:** The physical address of the sender's network card.
- **Type/Length (EtherType):** Indicates the type of upper-layer protocol encapsulated in the frame (e.g., 0x0800 for IPv4, 0x0806 for ARP).
- **Data (Payload):** The actual content, which can be an IP packet, an ARP message, etc. The minimum data size in an Ethernet frame is 46 bytes (padding bytes are added if the content is smaller).
- **Frame Check Sequence (FCS):** An error-detection code to ensure the frame was not corrupted during transmission.

### ARP Table (ARP Cache)

The ARP table (Address Resolution Protocol) is a mapping table stored in memory (a cache) on each device in the network. Its role is to map IP addresses (Layer 3 logical addresses) to MAC addresses (Layer 2 physical addresses).

**How the ARP table works:**

- **ARP Request:** If a device needs the MAC address for an IP address it does not know, it sends an ARP request as a broadcast on the local network.
- **ARP Reply:** The device that owns the requested IP address responds with its MAC address. This reply is sent as a unicast frame.
- **ARP Table Update:** The device that initiated the request adds the IP-MAC entry to its ARP table for future communications.

```c
struct ethhdr {
    unsigned char   h_dest[ETH_ALEN];   /* Destination Host Address */
    unsigned char   h_source[ETH_ALEN]; /* Source Host Address      */
    unsigned short  h_proto;            /* Protocol type            */
};

struct ether_arp {
    struct arphdr   ea_hdr;     /* ARP hrd & proto et al */
    u_char          arp_sha[ETH_ALEN];  /* Sender hardware address */
    u_char          arp_spa[4];         /* Sender protocol address */
    u_char          arp_tha[ETH_ALEN];  /* Target hardware address */
    u_char          arp_tpa[4];         /* Target protocol address */
};

struct arphdr {
    unsigned short  ar_hrd;     /* Format of hardware address   */
    unsigned short  ar_pro;     /* Format of protocol address   */
    unsigned char   ar_hln;     /* Length of hardware address   */
    unsigned char   ar_pln;     /* Length of protocol address   */
    unsigned short  ar_op;      /* ARP opcode (command)         */
};
```

### OSI Model

**Layer 1: Physical Layer**  
Responsible for transmitting raw bits over the physical medium (cables, Wi-Fi, fiber optics).  
Examples: RJ45 connectors, volts, Hz, bits, radio waves, Ethernet cables.

**Layer 2: Data Link Layer**  
Ensures error-free data transmission over a direct link and manages physical addressing.  
Examples: MAC addresses, Ethernet frames, switches, PPP, ARP.

**Layer 3: Network Layer**  
Manages logical addressing and routing of packets across different networks.  
Examples: IP addresses, routers, IP protocol.

**Layer 4: Transport Layer**  
Provides reliable end-to-end communication between applications, segments data, and ensures correct order.  
Examples: TCP, UDP, port numbers.

**Layer 5: Session Layer**  
Establishes, manages, and terminates communication sessions between applications.  
Examples: RPC, NetBIOS.

**Layer 6: Presentation Layer**  
Ensures data format compatibility, translates, compresses, and encrypts data.  
Examples: JPEG, ASCII, SSL/TLS.

**Layer 7: Application Layer**  
Provides network services directly to software applications, closest to the user.  
Examples: HTTP, FTP, SMTP, DNS.

## Sources

- [Man-in-the-middle-attack](https://datatracker.ietf.org/doc/html/rfc7042)
- [Address Resolution Protocol](https://en.wikipedia.org/wiki/Address_Resolution_Protocol)
- [rfc826](https://datatracker.ietf.org/doc/html/rfc826)
- [rfc7042](https://datatracker.ietf.org/doc/html/rfc7042)
- [NetworkConfiguration](https://wiki.debian.org/NetworkConfiguration)
- [git that i found usefull](https://github.com/BredyByte/ft_malcolm)

