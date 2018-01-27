Welcome to thelinkbox, a member of the CQiNet family.

http://CQiNet.sourceforge.net

Support: http://groups.yahoo.com/group/thelinkbox/

Thelinkbox is an voice over IP (VoIP) radio linking package for ham radio
operators that runs under FreeBSD and Linux, and hopefully other Posix 
compatible operating  systems.  It does not and probably never will run 
under Windows.

Thelinkbox is basically thebridge conferencing software with additional code 
to interface to radios. Although the primary purpose is for VoIP linking of a 
limited number of  repeaters "off grid", i.e. separate from the EchoLink and 
IRLP networks it can also be used on the Echolink or IRLP networks when 
properly configurated.  

Unlike some other VoIP systems thelinkbox also supports multiple radio 
ports. The ultimate goal is for thelinkbox to become a full featured
multiport hub or repeater controller as well as a VoIP application. I
believe the use of USB audio dongles under Linux will allow a modern
PC to support a larger number of radio ports than are likely to be
needed by even the largest repeater groups (> 16 ports), although I
have yet to test the limit in practice.


Here are some features:

1. Support for multiple simultaneous connections using different
codecs and protocols. Currently tlb supports ADPCM and GSM codecs and
the Echolink, RTP, and Speak freely protocols. When multiple sources
are active the audio is mixed.

2. The number of VoIP connections is limited only by system RAM and
available bandwidth.

3. The number of physical radio ports is limited only by available hardware, 
OS hardware support and CPU processing power.

4. All ports are completely independent: Each port may have a unique
set of DTMF commands. Each port can be connected to a distinct VoIP clients.

5. A full cross point matrix of connections between VoIP clients and
RF ports is supported.

6. Does not require the use of a central server or authentication
authority, all that is required is internet connectivity.

7. Support for either prerecorded PCM audio announcements or external
text to speech systems such as Festival.

8. CW and voice IDs.

9. Flexible general purpose telemetry tone generator that can be assigned
to various events and/or used by scripts.

10. "Permanent" connections, if a link is lost tlb will keep trying to
reconnect indefinitely.

11. Builtin software based DTMF and CTCSS encoders and decoders.

12. Open source software written in C and C++.

13. Support for inexpensive USB audio dongles including PTT, 
COS and CTCSS sense and frequency control.


#############################################
Updating from previous versions of thelinkbox
#############################################

New versions of thelinkbox are usually backwards compatible with configuration 
files from earlier releases HOWEVER new configuration file variables are added 
frequently.  Since the sample tlb.conf.sample file includes documentation on 
features which are not mentioned elsewhere it is worthwhile to review the 
sample configuration file with each release to discover new capabilities which 
you may want to take advantage of.  


###########
Portability
###########

A quick word on portability:  I've tried to the best of my ability and means
to make thelinkbox as portable as possible.  I've tested it under several
versions of FreeBSD and Linux, however *ALL* testing has been on the Intel x86
architecture.  

I've found that sound programming on Linux platforms can be very problematic.  
Results are very dependent on the sound board and its driver.  At least 50% of 
the sounds cards I've tested with had some issues that required special 
workarounds such as extra driver options.  Additionally for portability 
thelinkbox uses the OSS sound system API supported by many platforms, not the 
Linux specific ALSA sounds system.  Luckily the ALSA sound system includes an 
emulated OSS API for backwards compatibility.

At this time the USB subsystem is Linux specific.  I tried to make the USB code
portable by using libusb but I've run into a show stopper on FreeBSD, namely
if the USB audio device is claimed by the kernel's audio driver then libusb
is unable to access the device.


###################
Building on a *nix:
###################

Executive summary: (the usual)
        "tar xzf thelinkbox-{VERSION}.tgz"
        "cd thelinkbox-{VERSION}"
        "./configure" (or "./configure --enable-usb")
        "make"
        <edit the configuration files>
        <test>
        "make install"
        <start the daemon>

Requirements:
The only known requirement to build thelinkbox is the GNU GCC C compiler, the
GNU GPP C++ compiler or something compatible, a make program and a Bourne 
shell to run the configure script. GNU make is not required, any old make 
should be fine. I tried to avoid the use of exotic compiler features, 
hopefully any version of GCC will work.  

If for some reason either configure or the make should fail please
send me the details and I'll try to help you correct the problem.

Extract:
The distribution tar file extracts into a version specific subdirectory so 
different releases do not conflict with each other.  Substitute the actual
version number for "{VERSION}" above.  The ".tgz" extension signifies a
Gzip'ed TAR file.

GNU configure:
Thelinkbox uses a GNU autoconfig generated configuration script to generate a 
site-specific Makefile that is then used to build thelinkbox.  The configuration
script is written to be as portable as possible by only assuming the 
availability of the most generic Bourne shell features.  A suitable shell 
should be available on just about every *nix system.  It will certainly be 
available on any system using other GNU tools.

The configuration script provides a great deal of flexibility in the way
the target program is built and installed, run "./configure --help" for the
gory details.  Luckily most of the features are seldom needed and running
configure without any options is usually sufficient.  

If you want to use an USB audio dongle for an interface specify the 
--enable-usb option when you run configure.  In this case will need to have 
the libusb and it's header files installed on your system.

If you have the GNU readline library (libreadline) you can configure to tlbcmd 
to use it by specifing the --with-readline switch.  The readline library
adds features such as command history and curses based line editing tlbcmd and
tlbchat.

Once the configure script has been run you should have a config.h and Makefile 
appropriate for your system.  

Make:
The generated makefile provides several useful targets, the default "all" 
builds thelinkbox and utilities.  The other frequently used targets include 
"install", "clean", "distclean", and "uninstall". 

There shouldn't be any errors or warnings displayed during the build process
for the core source files.  I've included several open source libraries with
thelinkbox to try to prevent "dependency hell", some of those libraries
generate warnings. If you get warnings or errors in the core source files 
I'd be interested in hearing about them.  

The final output of the build process is the thelinkbox daemon "tlb" which is 
built in the linkbox subdirectory.  We'll test it before installing it.  
Before we test it we need to edit the configuration files.

########################
thelinkbox Configuration
########################

A least two text files "tlb.conf" and "tlb.cmds" that are needed to configure 
thelinkbox.  The first file can contain all of the configuration necessary 
other than the mapping of DTMF digits to actions that are taken when the 
DTMF digits are received. The second file contains the mapping of DTMF digits 
to commands.

The port configuration can be included in the main file if desired, but
it's probably cleaner and less confusing to have one configuration file
per port.  The port configuration files are then included from the main
configuration file by use of the "include" directive.

The files files "tlb.conf.sample" and "tlb.cmds.sample" are provided as 
a starting point for your configuration files.  Since the sample files
include documentation on features that are not mentioned elsewhere it is 
worthwhile to review the sample configuration file with each release to 
discover new capabilities which you may want to take advantage of.  

There are sample port configuration files for compatible interfaces included
in the release, just make N copies of that file that matches the type
of interface you will be using and them modify them as needed.

Fire up your favorite editor and change the various variables to appropriate 
values.  Refer to the comments in the file for guidance.  

--- snip ---
% cd /usr/local/etc
% cp tlb.conf.sample tlb.conf
% cp tlb.cmds.sample tlb.cmds
% cp iMic.conf.sample Port1.conf
% vi tlb.conf
% vi tlb.cmds
% vi Port1.conf
% cp irlp.conf.sample Port2.conf
% vi Port2.conf
% cp cm108.conf.sample Port3.conf
% vi Port3.conf
--- snip ---

Lines beginning with ';' or '#' are comments, if you decide to set any of the
optional settings be sure to delete leading ';' character before the 
configuration variable.

Since daemons are not connected to any console they must communicate with 
the sysop in some other manner.  As with most *nix daemons thelinkbox can use 
the syslog system.  Thelinkbox uses "LOG_LOCAL5" as the facility when opening 
the log and generates messages with priorities LOG_INFO, LOG_WARNING and 
LOG_ERR.  It's up to the user to decide which message if any he wants to log,
but unless you are a psychic I would strongly suggest that at least the LOG_ERR 
messages be logged.
 
Thelink box can also generate a local log file without using the syslog system.
This has many advantages and is the recommended method.  The configuration
file variable LogFileRolloverType controls the logging method.

##################################
Determining physical USB Addresses
##################################

If you are not using multiple USB audio dongles you can safely ignore this
section.

Since most USB audio dongles do not have a unique serial numbers we must 
resort to configuring thelinkbox using physical USB addresses.  This means 
the physical port on the PC or USB hub that the USB audio dongle is plugged
into determines the configuration address. Unfortunately it is not 
easy to determine this address without more documentation than is usually 
available for PCs.  Even if documentation were available determining the 
correct address would be tedious and error prone. 

Luckily there's an easier way!

1. Configure thelinkbox for a single USB port.  Do no specify either
AudioDongleSN or AudioDongleAdr.

2. Connect a compatible dongle to ONE port you wish to use.  It's easy for 
the linkbox to find the port to use when there's only one dongle by searching 
all USB buses until a device is found.
  
3. Start thelinkbox in debug mode.  Hopefully thelinkbox will find the dongle
and display it's address.  Make a note of the address displayed. A sticky 
label next to the port might be a good idea.

--- snip ---
[root@localhost ~]# /root/tlb -d -f /home/skip/tbd/tbd.conf.440
thelinkbox Version 0.13 compiled Dec 14 2007 16:24:36
16:40:07 thelinkbox V 0.13 compiled Dec 14 2007 16:24:36 initializing
16:40:07 EndPointInit: Initializing 440 port (0)
16:40:07 EndPointInit: called, TxKeyMethod: 5, RxCosMethod: 6
16:40:07 UsbInit: found USB device @ 1-1.2
16:40:07 UsbInit: USB audio device for 440 is /dev/dsp2
16:40:07 UsbInit: Changing audio device from auto to /dev/dsp2
16:40:07 FindDevByAdr: event device for "1-1.2" is /dev/input/event3
16:40:07 Fragsize: 256, fragstotal: 5, bytes: 1280
16:40:07 EndPointInit: returning 0
--- snip ---

In this example the AudioDongleAdr that was discovered is 1-1.2.

4. Hit control C to shutdown thelinkbox.

5. Disconnect the dongle from the port just found and move it to the next
port.  Repeat steps 3 to 5 until the addresses for all of the ports that will
be used have been determined.

6. Configure thelinkbox for multiple ports specifying the addresses found
for AudioDongleAdr.  


#######
Testing
#######

The daemon has two command line switches to aid testing.  The first switch -f
specifies where the tlb.conf configuration is located.  

The second switch -d enables debug mode, causing the daemon to run in the
foreground as a user process while displaying debug information on the screen.
The debug switch may be used multiple times to increase the detail level of
the information displayed up to a maximum of three times.

For our purposes a single -d suffices. 

--- snip ---
[root@localhost ~]# /root/tlb -d -f /home/skip/tbd/tbd.conf.440
thelinkbox Version 0.33 compiled May 14 2008 16:31:19
6:45:49 thelinkbox V 0.33 compiled May 14 2008 16:31:19 initializing
6:45:49 EndPointInit: Initializing 440 port
6:45:49 EndPointInit: called, TxKeyMethod: 5, RxCosMethod: 6
6:45:49 UsbInit: found USB device "440" @ 1-2.2
6:45:49 UsbInit: USB audio device for 440 is /dev/dsp2
6:45:49 UsbInit: Changing audio device from auto to /dev/dsp2
6:45:49 FindInputDevBySn: event device for "440" is /dev/input/event3
6:45:49 AudioInit#1586: opening "/dev/dsp2"
6:45:49 Fragsize: 256, fragstotal: 5, bytes: 1280
6:45:49 EndPointInit: returning 0
GenAVRS(): sending string:
)EL-wb6ymh!3345.88NE11820.19W0PHG8660/449220/131 On  @0645
PullerLoginAck(): Client 8 successfully updated status.
6:45:50 Msg from EchoLink: EchoLink Server v2.5.996
6:45:50 Msg from EchoLink:
6:45:50 Msg from EchoLink: ECHO4: Scottsdale, AZ USA
Full station list downloaded successfully, 8166 stations listed.
--- snip ---

The message beginning with "PullerLoginAck():" indicates that the daemon 
was successful in logging into the EchoLink directory server.

The last message is the real good news, our directory request returned 7904
stations.  If your callsign or password were not recognized no stations would
be listed and you would see an error message from EchoLink simular to the
following:

--- snip ---
6:47:01 Msg from EchoLink: INCORRECT PASSWORD
6:47:01 Msg from EchoLink:
6:47:01 Msg from EchoLink: Please check the password
6:47:01 Msg from EchoLink: and try again. If you have
6:47:01 Msg from EchoLink: forgotten it, see the Validation
6:47:01 Msg from EchoLink: section at www.echolink.org.
Full station list downloaded successfully, 0 stations listed.
--- snip ---

If you are not able to get a station list, check your callsign and password
in the configuration file.  You might also want to rerun the test using more 
"-d" switches to help determine what when wrong.
        
NB: thelinkbox makes NO attempt to control the sound card's mixer.
You'll need to set levels manually with the mixer utility of your choice.

To test run thelinkbox in debug mode "linkbox/tlb -d -f tlb.conf". Log
messages will be displayed on the console. Key a radio and hopefully
you'll see "COS detected". Press some touchtone buttons and hopefully
you'll also see them displayed.

--- snip ---
[root@localhost ~]# /root/tlb -d -f /home/skip/tbd/tbd.conf.440
thelinkbox Version 0.13 compiled Dec 14 2007 16:24:36
16:48:41 thelinkbox V 0.13 compiled Dec 14 2007 16:24:36 initializing
16:48:41 EndPointInit: Initializing 440 port (0)
16:48:41 EndPointInit: called, TxKeyMethod: 5, RxCosMethod: 6
16:48:41 UsbInit: found USB device @ 1-1.2
16:48:41 UsbInit: USB audio device for 440 is /dev/dsp2
16:48:41 UsbInit: Changing audio device from auto to /dev/dsp2
16:48:41 FindDevByAdr: event device for "1-1.2" is /dev/input/event3
16:48:41 Fragsize: 256, fragstotal: 5, bytes: 1280
16:48:41 EndPointInit: returning 0
GenAVRS(): sending string:
)EL-wb6ymh!3345.88NE11820.19W0PHG8660/449220/131 On  @1648
PullerLoginAck(): Client 6 successfully updated status.
Full station list downloaded successfully, 7910 stations listed.
PollCOS: Node 440 COS active
PollCOS: Node 440 COS inactive
16:48:47 DecodeDTMF: Decoding "123"
16:48:47 RF user executing command "say I'm sorry dave I can't do that"
--- snip ---

[much more to come ... stay tuned!]

############
Installation
############

Thelinkbox is designed to run as a system daemon, i.e. a background program 
that's loaded automatically by the system as part of the bootup process. 

If you are not familiar with the system startup scripts or you are not
comfortable starting thelinkbox automatically you can always start thelinkbox 
daemon manually when desired.  Starting thelinkbox automatically is primarily 
needed when the host is unattended and it is desired to run thelinkbox 24/7.
Refer to the "Running without Root access" section if you would rather not
modify your system's startup behavior.

Unfortunately my installation rules and knowledge are not complete enough about
all of the various *nix variations to complete the installation without manual
assistance.

Most Posix operating systems start system daemons using approaches similar
to either FreeBSD (i.e. the BSD camp) or RedHat (i.e. the System V camp). 
Scripts to install thelinkbox on FreeBSD and RedHat Linux have been provided.
Start with the scripts that are the closest match to your system and then 
modify them if necessary.  I will be happy to include installation scripts 
for other operating system that are sent to me with future releases.

Configuration files for daemons are kept in different places on different
systems.  It doesn't really matter where the configuration file is as long
as tlb can find it.  If you want to put it somewhere other than where
the GNU autoconf tools think it belongs just specify the full configuration 
file path on the command line using the -f switch.

FreeBSD

Local (not part of the standard distribution) daemons on FreeBSD systems are
started by placing a shell script /usr/local/etc/rc.d directory with an ".sh"
extension.  During startup each script is called with an argument of "start".
During system shutdown they are called again with an argument of "stop".

If you are running on FreeBSD run the installation scripts from the FreeBSD
subdirectory.  The installation script will copy tlb, tlb.conf.sample and
tlb.sh into the appropriate subdirectories. You will need to be root to 
when running the installation script.

--- snip ---
%cd FreeBSD
%su
Password:
fastbsd# ./install
+ cd ..
+ make install
Making install in src
/bin/sh ../config/mkinstalldirs /usr/local/libexec
  /usr/bin/install -c  tlb /usr/local/libexec/tlb
/bin/sh ./config/mkinstalldirs /usr/local/etc
 /usr/bin/install -c -m 644 ./tlb.conf.sample /usr/local/etc/tlb.conf.sample
+ cd FreeBSD
+ cp tlb.sh /usr/local/etc/rc.d
fastbsd# 
--- snip ---

Since the installation process only copies tlb.conf.sample (to prevent 
accidents when thelinkbox is updated in the future) we must manually copy
our configuration file to the "standard place":

--- snip ---
$ cp tlb.conf /usr/local/etc
--- snip ---


RedHat Linux

If you are running on RedHat Linux run the installation scripts from the
RedHat subdirectory.  The installation script will copy the tlb executable,
tlb.conf.sample, and tlb shell scripts into the appropriate subdirectories.  
It will then create links from /etc/rc.d/rc*.d subdirectories the 
/etc/rc.d/init.d/tlb shell script to start tlb in run levels 2, 3, 4 and 5 and 
to stop tlb in run levels 0, 1 and 6.  You will need to be root to when 
running the installation script.

--- snip ---
$ su
Password: 
[root@linux73 RedHat]# ./install
+ cd ..
+ make install
Making install in src
make[1]: Entering directory `/home/skip/thelinkbox-0.10/src'
make[2]: Entering directory `/home/skip/thelinkbox-0.10/src'
/bin/sh ../config/mkinstalldirs /usr/local/libexec
  /usr/bin/install -c  tlb /usr/local/libexec/tlb
make[2]: Nothing to be done for `install-data-am'.
make[2]: Leaving directory `/home/skip/thelinkbox-0.10/src'
make[1]: Leaving directory `/home/skip/thelinkbox-0.10/src'
make[1]: Entering directory `/home/skip/thelinkbox-0.10'
make[2]: Entering directory `/home/skip/thelinkbox-0.10'
/bin/sh ./config/mkinstalldirs /usr/local/etc
 /usr/bin/install -c -m 644 ./tlb.conf.sample /usr/local/etc/tlb.conf.sample
make[2]: Nothing to be done for `install-data-am'.
make[2]: Leaving directory `/home/skip/thelinkbox-0.10'
make[1]: Leaving directory `/home/skip/thelinkbox-0.10'
+ cd RedHat
+ ln -s /usr/local/libexec/tlb /usr/sbin
+ '[' '!' -d /etc/rc.d/init.d ']'
+ cp tlb /etc/rc.d/init.d
+ ln -s /etc/rc.d/init.d/tlb /etc/rc.d/rc0.d/K73tlb
+ ln -s /etc/rc.d/init.d/tlb /etc/rc.d/rc1.d/K73tlb
+ ln -s /etc/rc.d/init.d/tlb /etc/rc.d/rc2.d/S73tlb
+ ln -s /etc/rc.d/init.d/tlb /etc/rc.d/rc3.d/S73tlb
+ ln -s /etc/rc.d/init.d/tlb /etc/rc.d/rc4.d/S73tlb
+ ln -s /etc/rc.d/init.d/tlb /etc/rc.d/rc5.d/S73tlb
+ ln -s /etc/rc.d/init.d/tlb /etc/rc.d/rc6.d/K73tlb
[root@linux73 RedHat]# exit
$
--- snip ---

Since the installation process only copies tlb.conf.sample (to prevent 
accidents when thelinkbox is updated in the future) we must manually copy
or configuration file to the "standard place":

--- snip ---
$ cp tlb.conf /usr/local/etc
--- snip ---


########
Commands
########

Since thelinkbox is based on thebridge conference server all of the command
for thebridge are also available on thelinkbox.  Please see the
Command section in the documentation for thebridge (README.tbd) for details.

A few commands are unique to thelinkbox, only those commands are 
documented here.  Most commands are applied to the current port which is
selected by setting the port.  (.port 2).

".dtmfdecode <command>"

The .dtmfdecode command allows a specified command to be interpreted as if
it were entered by the RF user.  

".id"

The .id command forces the nodes ID to be sent.

".link [-m] [-p] <destination> [<source> ...]"

The link command is used to connect nodes (RF ports and VoIP connections) 
together.  Each linkage is bidirectional unless the -m (monitor) switch is 
used. For example if the system is a remote base and port "440" is it's 
input and port "144" is the remote 2 meter radio the command ".link -m 440 144" 
would put the remote base into 2 meter monitor mode.  i.e. traffic on 2 meters 
would be repeated to 440, but traffic on 440 would not be repeated to 2 
meters.  The command ".link 440 144" would put the remote base into 2 
meter transceive.

The -p command line switch specifies a permanent link.  Permanent links
are not unlinked by .unlink all, .unlink rf, .unlink voip, or .unlink <port> 
commands.  The link can only be removed by an explicit .unlink command that 
specifies both ports.

The link command can also be used to link VoIP connections to RF ports. For
example to link an *EXISTING* VoIP connection from W1AW to the "440" port:
.link w1aw 440 (or .link 440 w1aw).  VoIP connections are created when a
remote node connects to our node or by use of the .connect command.

".link"
To generate a list of current port linkages run the .link command without
arguments.

".users [-b] [-c] [-q] [-t] [-T] [-v] [?]"
  
The .users command lists the callsign of all VoIP connections order of login 
along with their attributes.  This command is particularly useful for net
control operators by enabling them to see more stations than will fit in the
EchoLink client's info window.

The meaning of the attribute characters may be display by the ".user ?" 
command.  They are as follows:

User attributes:
0 - User is usign the Speak Freely protocol
1 - User is using the RTP protocol
A - User is logged in as an administrator.
a - User is using the  ADCPM codec
B - User is running theBridge conference or thelinkbox.
C - User is a linked conference other than thebridge.
c - User's chat text is been suppressed
F - User is playing a file (using the .play or .test commands).
f - User is Full duplex
I - User is Isolated (not In conference)
K - User's has been Kicked (being disconnected).
L - User is a Lurker.
m - User's audio is muted.
M - User's audio and text are muted.
P - User is a permanent connection (connected by a .connect command).
R - User is in Receive only mode (being monitored)
S - User is logged in as a sysop.
T - User is currently Talking.
u - User is running the uLaw codec
x - User connection is inactive
! - User is an old version of thebridge which sent SDES packets containing 
    a private "txt" extension field.
    
The -b (bare) switch suppresses the display of user attributes.
The -c switch displays the amount of time the user has been connected.
The -q switch suppresses logging of the .user command.  This is useful when
the .user command is used by AJAX scripts to prevent the log from being
filled with less than interesting information.
The -t switch displays the amount of time since user last transmitted.
The -T switch sorts the user list by time since last transmission.
The -s switch displays the user list in the same format as is presented
       to EchoLink users on the right hand side of their client.
The -v switch displays the version number of user's client


".unlink all"
".unlink rf"
".unlink voip"
".unlink <destination> [<source>...]"

The unlink command is (much a you might expect) the reverse of the .link
command, it desolves links established with the .link command.  The "all" 
argument removes all links between both ports and VoIP connections.  
The "rf" argument removes all links between RF ports, but leaves links between
ports and Voip connections intact.  The "voip" argument removes all links
between ports and Voip connects, but leaves links between ports intact.
If the argument is a single node name then all links to that node are desolved.
Finally specific links can be desolved by specifing both the destination and 
source names.

".pcm record <test point>"
".pcm close <test point>"
".pcm"

The .pcm command allows raw PCM data from various points in the signal 
path for the current port to be saved to disk.  This command is primarily an 
debug aid. When the .pcm command is run without arguments a list of 
available test points is displayed.

".rxlevel"
".rxlevel -c"
".rxlevel -q"

The .rxlevel command to displays received audio level statistics from 
physical ports or VoIP connections.  This command displays the value of 
the minimum and maximum samples, the running total, the RMS level and the 
number of samples that were monitored.  The values are cleared after each 
.rxlevel command and then new values are accumlated for about one second.  
The -c switch enables a continous display of levels several times a second 
until another .rxlevel command is entered.
The -q switch suppresses the column header line.  For example:
  tlb> .rxlevel
  Min     Max     Total   RMS     Samples
  -8230   7167    -46     4256    512

The maximum possible value for 16 bit sound cards is 32767, the minimum
value is -32768.  There is no formal standard value for "full modulation",
but examining the reference IRLP and EchoLink audio recordings shows that
peak "modulation" is about 80% of full scale or about +/-26000. 
 
".script <path>"

The .script command allows any program or script to be executed.  
This command is only available for use by the DTMF command parser.  
This provides a very powerful way to extend the functionality of thelinkbox
by the addition of user programs.

".sendbeacon [-a] [-x]"
The .sendbeacon command is used to send an AX25 broadcast packet or an update 
to the APRS-IS system.

The -x switch (the default if no switch is specified) sends an AX25 broadcast 
packet to the selected port. The contents of the packet are defined by the 
port's Ax25BeaconPath, Ax25BeaconText, and Ax25BeaconAPRS configuration 
variables.  The beacon can also be sent automatically by setting the 
port's Ax25BeaconInterval variable. Automatic beacons sent when 
Ax25BeaconInterval expires are only sent when there is no other activity 
on the transmitter and it will not cause the transmitter to keyup.  In other 
words automatic beacons are sent at the end of the transmitter's hangtime 
just before the transmitter would have been unkeyed.

The -a switch forces an status update to be sent to the APRS-IS system if
APRS-IS support is enabled.


".shutup"

The .shutup command kills any announcements for the RF user that are queued. 

".say [-c] [-s] <text phrase>"
".say [-c] <wavefile>"

The .say command generates audio for the currently selected port.  If 
thelinkbox has been configured to use an external voice synthesizer then
any text phrase is may be used.  If the linkbox is not configured to use
a voice synthesizer then thelinkbox attempts to find prerecorded PCM audio 
files containing audio matching the requested phrase. 

The -s option specifies that the <text phrase> should be treated as a callsign
i.e. spelled out rather than spoken. 

".sweep <frequency>"
".sweep <frequency> <end frequency>"
".sweep <frequency> <end frequency> <sweep time>"
".sweep <frequency> <end frequency> <sweep time> <number of sweeps>"
".sweep <frequency> <end frequency> <sweep time> <number of sweeps> <level>"

The .sweep command is used to generate an audio test tone or audio frequency
sweep for testing transmit level on an RF port.  The level is the maximum
value of the generated PCM wave so it has the range of 0 to 32768.  We 
(the VoIP community) badly need to coordinate on what constitutes 100% 
modulation.  If there is a standard I'm unaware of it.


.tonegen [ID <id> ] [TF1 <freq> [EF1 <freq>]] [TL <level>] [TF2 <freq>]
         [TL2 <level>] DUR <duration> [RPT <count>] : ...

.tonegen [ID <id> ] [TF1 <freq>] [TL <level>] [WPM <speed> ] CW <cw message>:

.tonegen [ID <id> ] [TL <row level>] [TL2 <column level> ] [DUR <active time>]
         [PAUSE <pause time>] DTMF <dtmf buttons> :

.tonegen [ID <id> ] FILE <path to file>:

.tonegen [ID <id> ] AX25 <packet data>:

.tonegen #<id>

.tongen !<id>

ID <number>    Assign a reference number the tone for future reference.

TF1 <freq>     Set Tone 1 frequency, 0 = no tone

EF1 <freq>     Set Tone 1 end frequency for sweeps (starts at TF1)

TL <level>     Set Tone 1 and 2 level

TF2 <freq>     Set Tone 2 frequency, 0 = no tone

TL2 <level>    Set Tone 2 level

DUR <duration> duration of tone segment or the DTMF digit active time 
               in milliseconds.
               
PAUSE <time>   The amount of silence in milliseconds between DTMF digits.

WPM <speed>    Set speed of CW message in words per minute.

CW  <cw msg>   Send specified message in morse code at speed specified by WPM
               using a tone frequency specified by TF1 and tone level specified
               by TL.
               
DTMF <dtmf>    Send DTMF tones <dtmf> with tone level specified by TL and TL2,
               active duration specified by DUR and interadigit time specified 
               by PAUSE.  If DUR is not specified the active duration is 
               defined by the configuration file variable DtmfEncodeDuration.  
               If PAUSE is not specified the intradigit time is defined by
               the configuration file variable DtmfEncodePause.
               
#<id>          Play saved ToneSpec <id> which was previously stored by the
               tonespec configuration variable.
!<id>          Cancel ToneSpec <id> if it is running

FILE path      Tone segment is contents of specified 8 Khz wav file.

RPT <count>    repeat pattern from the beginning or last rpt count times.

The .tonegen command is used to generate tones for various purposes such as
courtesy tones, command acknowledgements, invalid command indication, 
timeouts, DTMF cover tones, busy tones, dial tones, etc.

Tone specifications can be stored for future use by providing an ID field and 
assigning them to the ToneSpec configuration file variable.  Stored tones can
be played back by referencing the ID for example .tonegen #123.  Additionally 
many internal events may automatically trigger tones which are assigned to the 
event. Tones that are assigned IDs may also be cancelled  before they complete 
by referencing the ID.  For example ".tonegen !123".

Tone specifications are made of one or more segments which are played in order.
Multiple segments are separated by ':'. Each tone segment must have an "DUR", 
"DTMF", "FILE", "CW" or AX25 field, all other fields are optional.

Tone segments may be repeated a specified number of times by the use of the 
RPT specification.  When encountered in a tonespec the RPT field will cause
the tone generation to repeat from the beginning or after the end of the
previous RPT sequence for the specified number of times.

Tone frequencies are set to zero after each segment is generated, but tone 
levels are only changed when explicitly modified.

In a DTMF string one or more '+' character may be used to extend the duration 
of the next digit.  For example +++*1234 = generates a '*' digit with 4 times 
the normal duration and before generating 1234 with the normal timings.  
The default DTMF digit timings are defines by the DtmfEncodeDuration and 
DtmfEncodePause configuration variables.  The default timings may be overidden
by specifying a DUR and/or PAUSE in the tone spec.

For the AX25 "tone" <packet data> is text used to generated a 1200 baud
AFSK AX.25 UI frame in the following format:

<From Callsign> ">" <To Callsign> [,<Digitpeater Callsign>] " " <packet text>

For example:

W1AW>QST Code practice tonight at 11:00 on 146.52.  The TL parameter may be 
used to adjust the amplitude of the generated AFSK tones.  By default the
tone level is set by the port configuration variable Ax25ToneLevel of the
currently selected port.

For the AX25 "tone" <packet data> is text used to generated a 1200 baud
AFSK AX.25 UI frame in the following format:

<From Callsign> ">" <To Callsign> [,<Digitpeater Callsign>] " " <packet text>

For example:

W1AW>QST Code practice tonight at 11:00 on 146.52.  The TL parameter may be 
used to adjust the amplitude of the generated AFSK tones.  By default the
tone level is set by the port configuration variable Ax25ToneLevel of the
currently selected port.

.port [<port name>]

The .port command selects the active port for commands such as .id, .say,
.tonegen, etc.  When run without arguments the .port lists all ports with
current port status.  For example:

tlb> port
Available ports:
  144 Rx
> 440 Tx
  WR7NV-R
tlb>



This shows that port "144" is receiving a signal and the port "440" is 
transmitting.  Additionally the '>' character displayed in front of the 440 
port indicates that it is the port currently selected for control.


.rxfrequency <frequency>

This command sets the frequency of the receiver of the selected port if
frequency control is supported by the hardware.  The frequency is specified
in Mhz (146.52).

.txoffset <transmitter offset from receiver frequency>

This command sets the transmitter's frequency in relationship to the receiver's
frequency.  The offset is specified in Mhz (-.600).  Changing .rxfrequency
does not effect the transmitter offset.

.rxtone <ctcss tone frequency>
.rxtone search
.rxtone any

This command sets the CTCSS tone of the receiver of the selected port if
CTCSS tone control is supported by the hardware.  The frequency is specified
in hz (131.8), a tone frequency of 0.0 selects carrier squelch.

If the port is configured to use the internal software CTCSS decoder rxtone 
may be set to "search" to have the CTCSS decoder determine the CTCSS
tone in use by a channel by monitoring.  When a CTCSS tone is decoded the
decoder exits search mode and begins decoding only the tone that was found. 
If thelinkbox is configured with an EventScript a "ctcss_decode" event 
is generated.

If the port is configured to use the internal software CTCSS decoder rxtone 
may be set to "any" to have the CTCSS decoder decode for any CTCSS standard
CTCSS tone.  This mode is primarily for testing.


.txtone <ctcss tone frequency>

This command sets the transmitter's CTCSS tone on the selected port if
CTCSS tone control is supported by the hardware.  The frequency is specified
in hz (131.8), a tone frequency of 0.0 disables CTCSS generation.


.frequency [<rx frequency> [,<tx offset> [,<tx tone> [,<rx tone>]]]]

This command allows all parameters of the selected port to be set in one
command.  If no arguments are specified the current values are displayed
if available.  If the transmitter offset is not specified it defaults to
0 (simplex).  If the transmitter tone is not specified it defaults to 
disabled. If the receiver tone is not specified it defaults to carrier 
squelch.


###############################
Bugs/Features/known limitations
###############################

1.  Consider this software to be Beta quality, it is very new and the paint is 
still wet.  One of the primary goals is stability approaching that of the 
operating system and in the FreeBSD or Linux case that says quite a bit.  At 
this point we're probably a long way from that goal.  At least when thelinkbox 
crashes under FreeBSD or Linux the operating system probably won't and we 
should get a core file that will allow the bug to found and hopefully fixed.  

2. Security is weak.  Currently EchoLink's security is based on passwords 
which are sent in clear text on over the Internet and the relative 
obscurity of the system.  Security through obscurity is no security at all! 

3. If the RunAsUser feature is used under Linux core dumps are disabled. This 
appears to be a security feature inherent in Linux.  If thelinkbox stops 
running for unexplained reasons please consider running it as root long enough 
to get a core dump to assist with correcting the problem.


######
Thanks
######

Special thanks to K7KAJ, N7LF, VK3JED and OH2LAK for helping me debug and 
test the pre-release versions of thelinkbox. 

Thanks to the fine folks at sourceforge for hosting this and literally 
thousands of other open source projects.  Please support the OSDN in anyway
you can!


################
How you can help
################

1. Report all bugs. Please provide details on the version of thelinkbox, the
operating systems, and operating conditions.  Log files and core dumps are 
not only helpful but in many cases essential.

2. Developers are welcome!  If you have ideas and would like contribute to the
development please contact me.

3. Documentation.  It's a well known fact that most programmers/engineers hate
documentation and are usually poor writers.  We need to document the programs
and protocols used by thelinkbox as well as other programs that will be written.
The CQiNet web site could certainly use some input and a webmaster. If your 
language of choice is not a programming language you can still help !

wb6ymh@gfrn.org Dec 9, 2012
