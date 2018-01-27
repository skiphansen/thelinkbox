#!/bin/sh
# Example DTMF command script to set the transmit, receive and CTCSS
# frequencies for a remote base.  
#
# This script is expected to be called with 3 or 4 digits of frequency
# information starting at the Mhz digit and the node name of the remote base.
# For example to select the frequency 146.94 on a 2 meter remote base the 
# user would enter "<frequency control prefix>694"
# or "<frequency control prefix>6940" where <frequency control prefix>
# are the DTMF digits assigned to the 2 meter frequency control command.  
# The prefix is handled by thelinkbox's DTMF command file and is not included 
# in the arguments to this script.
# For example if the <frequency control prefix> is "123" then tlb.cmds
# would have the following entries:
#
# 123??? = script ./freqctrl.sh $2 144
# 123???? = script ./freqctrl.sh $2 144
#
# If the selected frequency is recognized as a repeater output then
# the remote base will be configured to use the repeater with any necessary
# CTCSS access tone.  Otherwise the remote base will be configured to
# operate simplex.  The speech synthesizer is used to announce the selected
# frequency.  If a repeater is recognized the repeater callsign and location
# are also announced.
#
# The example data is for the greater Los Angeles area.
# $Id: freqctrl.sh,v 1.2 2008/12/30 17:41:12 wb6ymh Exp $

time=`date "+%m/%d %R:%S"`
echo "$time: decoding $1 from $TLB_PORT" >> freqctrl.log

case "$1" in
4505)
   freq="144.505,+.6,100.0"
   call="W6COH"
   qth="Hemet"
   ;;
4855)
   freq="144.855,+.6,0"
   call="W6LMU"
   qth="Loyola Marymount University"
   ;;
4895)
   freq="144.895,+.6,151.4"
   call="N6WZK"
   qth="Keller Peak"
   ;;
4895)
   freq="144.895,+.6,186.2"
   call="KF6ZTY"
   qth="Palos Verdes"
   ;;
5120|512)
   freq="145.12,-.6,131.8"
   call="WA6ISG"
   qth="Running springs"
   ;;
5140|514)
   freq="145.14,-.6,131.8"
   call="KM6DF"
   qth="Santa Maria"
   ;;
5180|518)
   freq="145.18,-.6,156.7"
   call="W6MPH"
   qth="Pasadena"
   ;;
5200|520)
   freq="145.20,-.6,103.5"
   call="N6AH"
   qth="Arcadia"
   ;;
5220|522)
   freq="145.22,-.6,114.8"
   call="N6SLD"
   qth="Victorville"
   ;;
5240|524)
   freq="145.24,-.6,118.8"
   call="K6LLU"
   qth="Heaps peak"
   ;;
5260|526)
   freq="145.26,-.6,103.5"
   call="AB6OQ"
   qth="Big Bear"
   ;;
5280|528)
   freq="145.28,-.6,127.3"
   call="K6QN"
   qth="Santa Monica"
   ;;
5300|530)
   freq="145.30,-.6,100.0"
   call="K6CPT"
   qth="Mount Disappointment"
   ;;
5320|532)
   freq="145.32,-.6,114.8"
   call="W6TRW"
   qth="Redondo Beach"
   ;;
5340|534)
   freq="145.34,-.6,107.2"
   call="K6JM"
   qth="Anza"
   ;;
5360|536)
   freq="145.36,-.6,131.8"
   call="WB6FLY"
   qth="Lompoc"
   ;;
5380|538)
   freq="145.38,-.6,100.0"
   call="N6RBR"
   qth="Palos Verdes"
   ;;
5400|540)
   freq="145.40,-.6,103.5"
   call="N6ME"
   qth="Fullerton"
   ;;
5420|542)
   freq="145.42,-.6,136.5"
   call="WB6NLU"
   qth="Newport Beach"
   ;;
5440|544)
   freq="145.44,-.6,136.5"
   call="N6USO"
   qth="Sunset Ridge"
   ;;
5460|546)
   freq="145.46,-.6,123.0"
   call="W6IER"
   qth="Sunset Ridge"
   ;;
5480|548)
   freq="145.48,-.6,127.3"
   call="W6JVA"
   qth="Paradise Hills"
   ;;
6025)
   freq="146.025,+.6,136.5"
   call="WB6ZTY"
   qth="Glendale"
   ;;
6085)
   freq="146.085,+.6,110.9"
   call="KA6AMR"
   qth="Duarte"
   ;;
6115)
   freq="146.115,+.6,91.5"
   call="K7GIL"
   qth="Victorville"
   ;;
6160|616)
   freq="146.16,+.6,146.2"
   call="W6JAM"
   qth="Tujunga"
   ;;
6175)
   freq="146.175,+.6,0"
   call="W6GNS"
   qth="Whittier"
   ;;
6265)
   freq="146.265,+.6,0"
   call="KF6FM"
   qth="Anaheim Hills"
   ;;
6355)
   freq="146.355,+.6,71.9"
   call="W6OKY"
   qth="Monterey Park"
   ;;
6385)
   freq="146.385,+.6,146.2"
   call="KE6TZG"
   qth="Keller Peak"
   ;;
6610|661)
   freq="146.61,-.6,103.5"
   call="K6FAM"
   qth="Sierra Peak"
   ;;
6640|664)
   freq="146.64,-.6,107.2"
   call="WB6WLV"
   qth="mount Otay"
   ;;
6655)
   freq="146.655,-.6,114.8"
   call="KE6UPA"
   qth="La Mirada"
   ;;
6670|667)
   freq="146.67,-.6,192.8"
   call="KD6AFA"
   qth="mount Lukens"
   ;;
6700|670)
   freq="146.70,-.6,131.8"
   call="N6HYM"
   qth="Gibralter Peak"
   ;;
6730|673)
   freq="146.73,-.6,107.2"
   call="W6NWG"
   qth="Palomar mountain"
   ;;
6730|673)
   freq="146.73,-.6,103.5"
   call="K0JPK"
   qth="Rio Hondo Peak"
   ;;
6745)
   freq="146.745,-.6,156.7"
   call="K6CHE"
   qth="Long Beach"
   ;;
6790|679)
   freq="146.79,-.6,114.8"
   call="K6SYU"
   qth="Fullerton"
   ;;
6805)
   freq="146.805,-.6,127.3"
   call="KN6OK"
   qth="Thousand Oaks"
   ;;
6820|682)
   freq="146.82,-.6,0"
   call="W6FNO"
   qth="Johnstone Peak"
   ;;
6850|685)
   freq="146.85,-.6,146.2"
   call="W6JBT"
   qth="Crestline"
   ;;
6880|688)
   freq="146.88,-.6,127.3"
   call="WA6ZTT"
   qth="Sulphur mountain"
   ;;
6895)
   freq="146.895,-.6,136.5"
   call="W6KRW"
   qth="Lomas Peak"
   ;;
6910|691)
   freq="146.91,-.6,91.5"
   call="KC6WGF"
   qth="Running springs"
   ;;
6925)
   freq="146.925,-.6,114.8"
   call="WA6TFD"
   qth="Santiago Peak"
   ;;
6940|694)
   freq="146.94,-.6,131.8"
   call="KE6FUZ"
   qth="Disneyland"
   ;;
6940|694)
   freq="146.94,-.6,91.5"
   call="WA6EFW"
   qth="Victorville"
   ;;
6970|697)
   freq="146.97,-.6,136.5"
   call="K6QEH"
   qth="Fullerton"
   ;;
6985)
   freq="146.985,-.6,127.3"
   call="WB6ZTX"
   qth="South mountain"
   ;;
7000|700)
   freq="147.00,+.6,131.8"
   call="WB6OBB"
   qth="Broadcast peak"
   ;;
7030|703)
   freq="147.03,+.6,103.5"
   call="K6RIF"
   qth="Palomar mountain"
   ;;
7060|706)
   freq="147.06,+.6,162.2"
   call="W6PWT"
   qth="Norco Hills"
   ;;
7075)
   freq="147.075,+.6,131.8"
   call="KG6TAT"
   qth="Santa Barbara"
   ;;
7090|709)
   freq="147.09,+.6,0"
   call="AA6DP"
   qth="Catalina"
   ;;
7120|712)
   freq="147.12,+.6,136.5"
   call="WA6NRB"
   qth="Glendale"
   ;;
7130|713)
   freq="147.13,+.6,107.2"
   call="W6NWG"
   qth="Palomar mountain"
   ;;
7150|715)
   freq="147.15,+.6,103.5"
   call="WR6JPL"
   qth="Pasadena"
   ;;
7180|718)
   freq="147.18,+.6,186.2"
   call="KK6NJ"
   qth="Chatsworth Peak"
   ;;
7195)
   freq="147.195,+.6,100.0"
   call="W6NVY"
   qth="Palos Verdes"
   ;;
7210|721)
   freq="147.21,+.6,131.8"
   call="KK6SS"
   qth="Apple Valley"
   ;;
7225)
   freq="147.225,+.6,94.8"
   call="K6DCS"
   qth="Castro Peak"
   ;;
7240|724)
   freq="147.24,+.6,67.0"
   call="W6MEP"
   qth="Duck mountain"
   ;;
7270|727)
   freq="147.27,+.6,100.0"
   call="WA6ZTR"
   qth="Mount Disappointment"
   ;;
7300|730)
   freq="147.30,+.6,123.0"
   call="WB6QHB"
   qth="Upland"
   ;;
7330|733)
   freq="147.33,+.6,131.8"
   call="K6BB"
   qth="Big Bear Lake"
   ;;
7420|742)
   freq="147.42,-0.945,107.2"
   call="WA6BGS"
   qth="La Mesa"
   ;;
7435)
   freq="147.435,-1.035,103.5"
   call="KG6DVO"
   qth="Sylmar"
   ;;
7450|745)
   freq="147.45,-1.035,162.2"
   call="W6FP"
   qth="Oat mountain"
   ;;
7465)
   freq="147.465,-0.96,103.5"
   call="W6VLD"
   qth="Huntington Beach"
   ;;
7615)
   freq="147.615,-.6,136.5"
   call="W6LY"
   qth="Laguna Woods"
   ;;
7645)
   freq="147.645,-.6,103.5"
   call="KC6OKB"
   qth="Big Bear Lake"
   ;;
7705)
   freq="147.705,-.6,141.3"
   call="N7RDA"
   qth="Oat mountain"
   ;;
7735)
   freq="147.735,-.6,100.0"
   call="KB6C"
   qth="Oat mountain"
   ;;
7765)
   freq="147.765,-.6,0"
   call="K6VGA"
   qth="Frazier Park"
   ;;
7885)
   freq="147.885,-.6,103.5"
   call="W6BII"
   qth="Brea"
   ;;
7915)
   freq="147.915,-.6,136.5"
   call="K6NX"
   qth="Anaheim"
   ;;
7930|793)
   freq="147.93,-.6,127.3"
   call="AD6SV"
   qth="Simi Valley"
   ;;
7945)
   freq="147.945,-.6,136.5"
   call="KF6JWT"
   qth="Oat mountain"
   ;;
7975)
   freq="147.975,-.6,114.8"
   call="WD6DPY"
   qth="Orange"
   ;;
4*|5*|6*|7*)
   #if nothing else matches then assume simplex
   mhz=`echo $1 | cut -c 1`
   decimal=`echo $1 | cut -c 2-`
   freq="14${mhz}.${decimal}"
   qth="simplex"
   ;;
esac

freq1=`echo $freq | cut -d ',' -f1` 

if [ "${call}x" != "x" -a "${qth}x" != "x" ]; then
   # we have both a callsign and a qth
   tlbcmd "say -s $freq1; say -s $call; say $qth;port $2; freq $freq"
elif [ "${call}x" != "x" ]; then
   # we have just a callsign
   tlbcmd "say -s $freq1; say $call;port $2; freq $freq"
elif [ "${qth}x" != "x" ]; then
   # we have just a qth
   tlbcmd "say -s $freq1; say $qth;port $2; freq $freq"
elif [ "${freq1}x" != "x" ]; then
   # we only have a frequency
   tlbcmd "say -s $freq1;port $2; freq $freq"
else
   # Say what?
   tlbcmd "say invalid frequency"
fi


