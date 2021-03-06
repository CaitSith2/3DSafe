# 3DSafe
*In-NAND PIN lock for 3DS*

3DSafe is an arm9loaderhax payload which will lock your sysnand with a PIN. The PIN request is displayed as soon as the 3DS is powered on. Because the 3DSafe payload is the A9LH stage1/stage2 payload, it is stored in NAND itself, not on the SD card. The PIN is also stored in NAND, so there is no way to edit or remove the PIN by removing the SD card or modifying files on it. After successfully entering the PIN, arm9loaderhax.bin is loaded from the SD card.

# If you forget your PIN
Because everything to do with 3DSafe is in NAND, you cannot remove the PIN lock or change the PIN until you have already got past the request for the PIN. For this reason, a bypass is included. This involves getting your console-specific OTP, placing it at /otp.bin on your 3DS SD card, and then booting. 3DSafe will detect the presence of the OTP file and bypass the PIN request, allowing you to change the PIN and boot the console.

# You must safeguard your PIN and your OTP.bin
I cannot stress this enough. If you install 3DSafe, forget your PIN and lose your OTP, your 3DS will be a brick. There is absolutely no way to circumvent the PIN request without the OTP.bin. The only thing you would be able to do in this situation would be to hardmod your 3DS and use the hardmod to write a NAND backup which does not have 3DSafe installed (or one in which you know the PIN). If you forget your PIN, lose your OTP.bin and don't have a NAND backup you can restore using a hardmod, your 3DS will be *permanently bricked*.

I reiterate: ***BEFORE*** **installing 3DSafe, make two NAND backups, verify that the md5sums match, do the same for your OTP, and then store your NAND backup and OTP in several safe locations. If you don't do this and forget your PIN, your 3DS is BRICKED.**

# Testing and disclaimer
I have tested this on my EUR n3DS. I make absolutely no guarantee that it will work for anybody else. Since you are writing these payloads to sysNAND, there is a possibility that you will brick your 3DS. I take absolutely no responsibility for this. Do not install this unless you know exactly what you are doing. I highly recommend that you take a NAND backup before installing this, and preferably have a hardmod before installing.

##How to Install
1. Make a NAND backup. This is essential, as if something goes wrong during installation you will be able to restore it (but you'll need a hardmod to do this)
2. Store your NAND backup safely along with your OTP. You will need these if you forget the PIN.
3. Download and extract the 3DSafe release, and copy payload_stage1.bin and payload_stage2.bin to the a9lh folder on the root directory of your 3DS SD card
4. Run SafeA9LHInstaller according to its instructions to install the payloads
5. Power off your 3DS and power back on, and you will be prompted to set a new PIN
6. After entering a PIN, you will be able to boot your 3DS
7. Reboot your 3DS and confirm that your PIN unlocks it
8. Copy your otp.bin to the root of your SD card and reboot your 3DS
9. When 3DSafe starts up, confirm that your OTP has successfully circumvented the PIN lock. This will ensure that you have a valid OTP to bypass the PIN lock if you ever forget your 3DSafe PIN
10. Delete the otp.bin from the root of your SD card. Make sure you have multiple backups of this file elsewhere than your 3DS SD card. It's only a small file, so I recommend emailing it to yourself, putting it on a Dropbox, uploading it to a web host, burning it to a disc, putting it on a memory stick, or as many of these as is possible.

##Possible future addition:

* Write NAND back to SysNAND from within 3DSafe if you forget the PIN and don't have OTP.bin (don't count on this though - make sure you have your OTP safe)

## Credits

This project is based on ShadowNAND by @RShadowhand, from which it is forked. All credit for the original payload is inherited from this project and the projects on which it is based in turn. The modifications in 3DSafe are by @mashers.

3DSafe incorporates parts of GodMode9 by d0k3 for reading and writing the PIN from/to NAND. Credit for the code in 3DSafe which is taken from GodMode9 and modified by mashers is given to d0k and the other contributors to the GodMode9 project. This includes the following components of 3DSafe:
* godmode.c
* godmode.h
* fatfs (modified to mount/read/write 3DS NAND partitions)
* nand

3DSafe also includes OTP verification routines from SafeA9LHInstaller by AuroriaWright.
