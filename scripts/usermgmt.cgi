#!/bin/bash
#-------------------------------------------------------------------
#  TheBridge Echolink Conference Server User Management Utility 
#
#  2006/12/26 KC4YOZ - Original, ported to BASH from REXX.
#  2006/12/27 KDJ    - HELP Panel, cosmetics.
#  2006/12/31 KDJ    - Implement "Send a text message".
#  2007/01/02 KDJ      Implement QRZ lookups.
#                      Neater control buttons for Mozilla 5+
#  2007/02/01 KDJ    - Support for Location/Info string changes..
#
#-------------------------------------------------------------------
#  Installation Default Settings
confname=                     # name of this conference (ex *LINUX*)
admin=                        # admin's callsign or quoted name
qrz=yes                       # leave blank to disable QRZ lookups
chginfo=no                    # enable/disable Info/Location changes
tbdcmd=/usr/local/bin/tbdcmd  # path to "tbdcmd" executable
#-------------------------------------------------------------------
echo 'Content-type: text/html'
echo ''
if [ "$QUERY_STRING" != "help" ]; then

#  Refresh Interval stuff..

  if [[ $QUERY_STRING == refresh=* ]]; then
#    Get the value, check the right button..
    IFS=$'='
    for rfp in $QUERY_STRING ; do rfin[r++]=$rfp ; done
    rftime=${rfin[1]}
    case $rftime in
      0)
        chk0=checked
        chk10=
        chk30=
        chk60=
        chk300=
        chk900=
        chk1800=
      ;;
      10)
        chk0=
        chk10=checked
        chk30=
        chk60=
        chk300=
        chk900=
        chk1800=
      ;;
      30)
        chk0=
        chk10=
        chk30=checked
        chk60=
        chk300=
        chk900=
        chk1800=
      ;;
      60)
        chk0=
        chk10=
        chk30=
        chk60=checked
        chk300=
        chk900=
        chk1800=
      ;;
      300)
        chk0=
        chk10=
        chk30=
        chk60=
        chk300=checked
        chk900=
        chk1800=
      ;;
      900)
        chk0=
        chk10=
        chk30=
        chk60=
        chk300=
        chk900=checked
        chk1800=
      ;;
      1800)
        chk0=
        chk10=
        chk30=
        chk60=
        chk300=
        chk900=
        chk1800=checked
      ;;
    esac
    IFS=$' \t\n'
  else    #  first time in, set default value and button..
    chk0=checked
    rftime=0
  fi

  echo '<html><head>'
  echo '<title>TheBridge Conference Server User Management</title>'
  echo '<!-- input parm ['$QUERY_STRING'] -->'
  if [ $rftime != 0 ]; then
    echo '<META HTTP-EQUIV="refresh"'
    echo ' content="'$rftime'; url=/cgi-bin/usermgmt.cgi?refresh='$rftime'">'
  fi
  echo '</head><body>'
  echo '<div align=left>'$(date "+%A %Y/%m/%d %T %Z")'</div>'
  echo '<div align=right><form>'
  hurl=/cgi-bin/usermgmt.cgi?help   # URL & Attrs for the Help panel..
  wop=toolbar=no,location=no,status=no,resizable=yes,scrollbars=yes,width=500,height=500
  echo '<input type=button value=" Help "'
  echo 'onClick=window.open("'$hurl'","help","'$wop'")></form></div>'
  echo '<center><h1>'$confname' Conference Server User Management</h1>'

#  Execute a command...

  if [ "$QUERY_STRING" != "" ]; then   #  we have a command...

    if [[ $QUERY_STRING == msgtxt=* ]]||[[ $QUERY_STRING == infotxt=* ]]; then  

#  Console text message or "Info" string changes...
      IFS=$'='            #  separate out the message text..
      for tmsg in $QUERY_STRING ; do tmss[h++]=$tmsg ; done
      instr=${tmss[1]}    #  input string..
      outstr=             #  output string..
      len=${#instr}       #  length of input..
      i=0                 #  byte pointer..
      while [ $i -lt $len ]; do {       #  now examine each byte..
        thischar=${instr:$i:1}
        if [ $thischar == % ]; then     #  hex junk from the URI..
          xbyte=${instr:$i+1:2}             #  grab the byte..
          tword=$(echo "\x"$(echo $xbyte))  #  prefix it..
          abyte=$(echo -e "$tword")         #  translate to ASCII..
          outstr=$(echo $outstr$abyte)      #  append the buffer..
          i=$(($i+3))                       #  bump the pointer..
        else
          if [ $thischar == + ]; then   #  plus is really a space..
            outstr=$(echo $outstr' ')
          else                          #  just take the character..
            outstr=$(echo $outstr$thischar)
          fi
          i=$(($i+1))
        fi
        };
      done
      if [[ $QUERY_STRING == msgtxt=* ]]; then
        if [ "$confname" == "" ]; then
          prefix="SYSOP>"
        else
          prefix=$(echo $confname">")
        fi
        statusmsg=$($tbdcmd -s message $prefix$outstr)
      else
        if [[ $QUERY_STRING == infotxt=* ]]; then
          statusmsg=$($tbdcmd -s info $outstr)
        fi
      fi
    else

      if [[ $QUERY_STRING == connect* ]]; then      # connect a station...
        IFS=$'='
        for tcmd in $QUERY_STRING ; do tcmd[h++]=$tcmd ; done
        if [ "${tcmd[1]}" != "" ]; then 
          statusmsg=$($tbdcmd -s connect ${tcmd[1]})
        fi
      else

        IFS=$'z'     # commands against a station in the list...
        for tcmd in $QUERY_STRING ; do tcmd[h++]=$tcmd ; done
        g=0
        while [ $g -lt $h ]; do {
          if [[ ${tcmd[g]} == =.* ]]; then  # --> separator
            statm=
            f=$(($g+1))  # --> command
            e=$(($g+2))  # --> callsign
            case ${tcmd[$f]} in
              mu)  # Mute
                statm=$($tbdcmd -s mute ${tcmd[$e]})
              ;;
              dc)  # Disconnect
                statm=$($tbdcmd -s disconnect ${tcmd[$e]})
              ;;
              db)  # Ban
                stata=$($tbdcmd -s disconnect ${tcmd[$e]})
                statb=$($tbdcmd -s ban add ${tcmd[$e]})
                statm=$(echo $stata$statb)
              ;;
              mp)  # Mute Persist
                statm=$($tbdcmd -s "mute -p ${tcmd[$e]}")
              ;;
              mo)  # Monitor
                statm=$($tbdcmd -s monitor ${tcmd[$e]})
              ;;
              um)  # Reset
                $tbdcmd -s delurk ${tcmd[$e]} >/dev/null
                stat1=$(echo ${tcmd[$e]}' de-lurked')
                stat2=$($tbdcmd -s monitor disable ${tcmd[$e]})
                stat3=$($tbdcmd -s unmute ${tcmd[$e]})
                $tbdcmd -s "unmute -p ${tcmd[$e]}" >/dev/null
                statm=$(echo $stat1$stat2$stat3)
              ;;
              ab)  # About
                statm=$($tbdcmd -s about ${tcmd[$e]})
              ;;
              qr)  # QRZ Lookup
                if [ "$qrz" != "" ]; then
                  callsign=${tcmd[$e]}
                  if [[ "${callsign:0:1}" == "*" ]]; then
                    statm=$(echo $callsign' cannot be looked up on QRZ.')
                  else
                    if [[ $callsign == *-* ]]; then
                      IFS=$'-'
                      for this in $callsign ; do tcall[a++]=$this ; done
                      qrzpop=${tcall[0]}
                    else
                      qrzpop=${tcmd[$e]}
                    fi
                    statm=$(echo 'QRZ query issued for '$qrzpop)
                  fi
                fi
              ;;
            esac
            statusmsg=$(echo $statusmsg$statm)
          fi
          g=$(($g+1))
        };
        done
      fi
    fi
    IFS=$' \t\n'
  fi

#  Status text goes here...

  if [ "$statusmsg" == "" ]; then 
    echo '<p>.'
  else
    echo '<p>'$statusmsg
  fi

#  QRZ PopUp Call..

  if [ "$qrz" != "" ]; then
    if [ "$qrzpop" != "" ]; then
      echo '<script type="text/javascript">'
      echo '<!--'
      echo 'window.open("http://www.qrz.com/'$qrzpop'","QRZ",'
      echo '"toolbar=no,location=no,status=no,resizable=yes,scrollbars=yes,width=800,height=600");'
      echo '//-->'
      echo '</script>'
    fi
  fi

#  Get and format the list of connected stations...

  var1=$($tbdcmd -s users)
  for this in $var1 ; do tbdu[i++]=$this ; done
  if [ $i == 1 ]; then  # no users, just a return code.
    echo '<h2>No stations logged in.</h2>'
  else  # got at least one.. make the table..
    echo '<form name="form">'
    echo 'Color Code: Normal '
    echo '/ <font color=orange>Admin</font> / <font color=blue>Conf</font> '
    echo '/ <font color=green>Perm</font> / <font color=brown>Muted</font> '
    echo '/ <font color=gray>Kicked</font> / <font color=purple>Monitor</font> '
    echo '/ <font color=cyan>Lurking</font> / <font color=red>Talking</font>'
    echo '/ <font color=pink>Playing</font>'
    echo '<table border="2" cellspacing="0"><tr><td align="center">Callsign</td>'
    echo '<td align="center">Command Options</td></tr>'

    j=1  # bump past the return code..
    while [ $j -lt $i ]; do {
      clr=black
      RFstn=NO ; lurker=NO  ; admin=NO  ; perm=NO    ; conf=NO 
      muted=NO ; talking=NO ; kicked=NO ; monitor=NO ; irlp=NO

      if [[ ${tbdu[j]} == *[0-9]. ]]; then  # --> user number
        k=$(($j+1))  # --> callsign
        l=$(($j+2))  # --> user flags
        if [[ ${tbdu[l]} == *C* ]]||[[ ${tbdu[l]} == *B* ]]; then   # Conference
          clr=blue
        fi
        if [[ ${tbdu[l]} == *A* ]]||[[ ${tbdu[l]} == *S* ]]; then   # Admin
          clr=orange
        fi
        if [[ ${tbdu[l]} == *L* ]]; then   # Lurker
          clr=cyan
        fi
        if [[ ${tbdu[l]} == *P* ]]; then   # Permanent
          clr=green
        fi
        if [[ ${tbdu[l]} == *M* ]]||[[ ${tbdu[l]} == *m* ]]; then   # Muted
          clr=brown
        fi
        if [[ ${tbdu[l]} == *K* ]]; then   # Kicked
          clr=gray ; kicked=YES
        fi
        if [[ ${tbdu[l]} == *R* ]]; then   # Monitor
          clr=purple
        fi
        if [[ ${tbdu[l]} == *T* ]]; then   # Talking
          clr=red
        fi
        if [[ ${tbdu[l]} == *F* ]]; then   # Playing a file
          clr=pink
        fi
        echo '<tr><td align="center"><font color='$clr'><b>'${tbdu[k]}'</b>'
        echo '</font></td><td align="center">'
        if [[ $kicked == NO ]]; then  # format the radio buttons..
          if [[ $HTTP_USER_AGENT == Mozilla/5* ]]; then
#  Firefox can handle Action Buttons...
            tdc='<td align="center"><button type="submit" name="'
            tdd='</button></td>'
          else
#  IE and Netscrape cannot..
            tdc='<td align="center"><input type=radio name="'
            tdd='</td>'
          fi
          echo '<table border="1" cellspacing="0"><tr>'
          echo $tdc'zdcz'${tbdu[k]}'z" value=.> Disc '$tdd
          echo $tdc'zdbz'${tbdu[k]}'z" value=.> Disc/Ban '$tdd
          echo $tdc'zmuz'${tbdu[k]}'z" value=.> Mute '$tdd
          echo $tdc'zmpz'${tbdu[k]}'z" value=.> Mute-P '$tdd
          echo $tdc'zmoz'${tbdu[k]}'z" value=.> Monitor '$tdd
          echo $tdc'zumz'${tbdu[k]}'z" value=.> Reset '$tdd
          echo $tdc'zabz'${tbdu[k]}'z" value=.> About '$tdd
          if [ "$qrz" != "" ]; then
            echo $tdc'zqrz'${tbdu[k]}'z" value=.> QRZ '$tdd
          fi
          echo '</tr></table>'
        fi
      echo '</td></tr>'
      fi
      j=$(($j+1))
    };
    done
    echo '</table>'

#  Form Buttons...  for IE and Netscrape..

    if [[ $HTTP_USER_AGENT != Mozilla/5* ]]; then
      echo '<input type=reset value=" Clear ">'
      echo '<b><input type=submit value=" Enter "></b>'
    fi
    echo '</form>'
  fi

#  Connect a station...

  echo '<hr><form><p><b>Connect a station: </b>'
  echo '<input type=text size=10 name=connect>'
  echo '<input type=reset value=" Clear ">'
  echo '<b><input type=submit value=" Enter "></b>'
  echo '</form>'

#  Text Message..

  echo '<hr><form><p><b>Send a text message: </b>'
  echo '<input type=text size=20 name=msgtxt>'
  echo '<input type=reset value=" Clear ">'
  echo '<b><input type=submit value=" Enter "></b>'
  echo '</form>'

#  "Info" String..

  if [ $chginfo == yes ]; then 
    echo '<hr><form><p><b>Change Info/Location text: </b>'
    echo '<input type=text size=20 name=infotxt>'
    echo '<input type=reset value=" Clear ">'
    echo '<b><input type=submit value=" Enter "></b>'
    echo '</form>'
  fi

#  Refresh Interval

  echo '<hr><form><table><tr><td><b>Refresh Interval:</b>'
  echo '<td><table border="2" cellspacing="0"><tr>'
  echo '<td><input type=radio name=refresh value=0 '$chk0'>None</td>'
  echo '<td><input type=radio name=refresh value=10 '$chk10'>10 sec</td>'
  echo '<td><input type=radio name=refresh value=30 '$chk30'>30 sec</td>'
  echo '<td><input type=radio name=refresh value=60 '$chk60'>1 min</td>'
  echo '<td><input type=radio name=refresh value=300 '$chk300'>5 min</td>'
  echo '<td><input type=radio name=refresh value=900 '$chk900'>15 min</td>'
  echo '<td><input type=radio name=refresh value=1800 '$chk1800'>30 min</td>'
  echo '</table><td><input type=submit value=" Set / Refresh ">'
  echo '</table></form></center>'
else

#  HELP panel...

  echo '<html><head><title>Conference User Management Help</title>'
  echo '<script type="text/javascript">'
  echo '<!--'
  echo 'window.focus();'
  echo '//-->'
  echo '</script></head><body>'
  echo '<div align=right><form>'
  echo '<input type=button value="Close Window"'
  echo 'onClick={self.close();}></form></div>'
  echo '<h1>User Management Help</h1>'
  echo '<p>The User Management panel lets you control all of the stations'
  echo 'connected to the '
  if [ "$confname" == "" ]; then
    echo 'Echolink conference server.'
  else
    echo $confname' conference server.'
  fi
  echo "<p>You'll see a"
  echo '<b><a href="#list">color-coded list</a></b> of station call signs and'
  echo 'a row of <b><a href="#control">control buttons</a></b> corresponding'
  echo 'to each station.  When you click on a control button'
  if [[ $HTTP_USER_AGENT != Mozilla/5* ]]; then
    echo 'and click the "Enter" button,'
  fi
  echo 'your command will take effect immediately.'
  echo '<p>You can also <b><a href="#outb">connect more stations</a></b> to the'
  echo 'conference, or <b><a href="#msg">send a text message</a></b> using this'
  echo 'panel.  <p>The status of your commands will be shown'
  echo 'on the panel just above the station list.<p><b>Note:</b> This'
  echo 'panel was designed for a screen resolution of 1024x768.'
  echo '<p><u><b><a name="list">The Station List</a></b></u>'
  echo '<p>The call signs are highlighted with color to give you an indication'
  echo "of each station's status.  Here are the details:"
  echo '<ul><li><b>Normal</b>: Just a normal Echolink station, nothing special'
  echo 'about it.  It may be a repeater, an RF link or a user on a PC.'
  echo '<li><font color="orange"><b>Admin</b></font>: The station is logged in'
  echo 'to this conference server as an <i>Administrator</i> or a <i>Sysop</i>.'
  echo '<li><font color="blue"><b>Conf</b></font>: The station is itself a'
  echo 'conference, either a <i>TBD server</i>, like this one, or an Echolink'
  echo 'station with multiple connections.'
  echo '<li><font color="green"><b>Perm</b></font>: A <i>permanent connection</i>'
  echo 'is the result of an <b><a href="#outb">outbound connect</a></b> request.  If'
  echo "the station's connection times out, the conference will attempt to re-connect."
  echo '<li><font color="brown"><b>Muted</b></font>: The station is <i>muted</i>,'
  echo 'it will hear any activity on the conference but is not able to transmit.'
  echo '<li><font color="gray"><b>Kicked</b></font>: The station has been'
  echo 'disconnected by the conference.'
  echo '<li><font color="purple"><b>Monitor</b></font>: The station can transmit'
  echo 'to the conference, but the conference will not transmit to the station.'
  echo '<li><font color="cyan"><b>Lurking</b></font>: The station invoked the'
  echo '<i>lurk</i> command to remove his call sign from the station lists'
  echo 'on the Echolink clients, but is still connected and can participate'
  echo 'in the conference as if it were a <i>normal</i> user.'
  echo '<li><font color="red"><b>Talking</b></font>: The station is currently'
  echo 'transmitting to the conference.'
  echo '<li><font color="pink"><b>Playing</b></font>: The station is playing a'
  echo 'file; either listening to a bulletin or using the .test command to play'
  echo 'back its own audio.  If the conference is set up to play a "welcome"'
  echo "message to connecting stations, they'll show up in pink momentarily"
  echo 'just after they connect.'
  echo '</ul>'
  echo '<p><u><b><a name="control">What the Control Buttons do</a></b></u>'
  echo '<ul><li><b>Disc</b>: Disconnects a station from the conference.  The'
  echo 'station can re-connect back in if it wants to.'
  echo '<li><b>Disc/Ban</b>: Disconnects a station and <i>bans</i> it so it'
  echo 'cannot re-connect.  Bans can only be reversed by the conference '
  if [ "$admin" == "" ]; then
    echo 'administrator.'
  else
    echo 'administrator, '$admin'.'
  fi
  echo '<li><b>Mute</b>: Stops a station from transmitting to the conference.'
  echo 'The station can disconnect and re-connect to un-mute itself.'
  echo '<li><b>Mute-P</b>: Mutes a station <i>persistently</i>, that is, the'
  echo 'station will stay muted even if it disconnects and re-connects.'
  echo '<li><b>Monitor</b>: The opposite of "Mute".  The conference will'
  echo 'receive what the station transmits, but cannot talk back to that'
  echo 'station.  This is useful for monitoring nets and space shuttle'
  echo 'missions.  If a repeater station needs to disconnect, it'
  echo 'will not disrupt the rest of the net.'
  echo '<li><b>Reset</b>: This function cancels the "Mute" and "Monitor"'
  echo 'functions, and also <i>de-lurks</i> (un-hides) a station which is'
  echo '<i>lurking</i> (connected, but not appearing in the Echolink'
  echo "clients' station lists)."
  echo "<li><b>About</b>: Shows the station operator's information."
  if [ "$qrz" != "" ]; then
    echo '<li><b>QRZ</b>: Performs a callsign lookup on the QRZ database.  The'
    echo 'QRZ information will pop up in a separate window.  The conference'
    echo 'administrator can disable QRZ lookups.'
  fi
  echo '</ul>'
  echo '<p><u><b><a name="outb">Outbound Connections</a></b></u>'
  echo "<p>You may connect a station to the conference by typing the station's"
  echo 'call sign into the text field following "<b>Connect a station:</b>"'
  echo 'and clicking the "<b>Enter</b>" button.'
  echo '<p>If the station does not immediately show up in the station list,'
  echo 'it is either offline or not accepting connections.'
  echo '<p><u><b><a name="msg">Send a Text Message</a></b></u>'
  echo '<p>Whatever you type into this text field will be displayed in the'
  echo 'text box of all connected Echolink clients.  You must have an'
  echo 'Echolink client to see any of the text messages.'
  if [ $chginfo == yes ]; then
    echo '<p><u><b>Change Info/Location Text</b></u>'
    echo "<p>You can modify the text shown next to the conference server's name on"
    echo 'the Echolink client.  Your change will take effect until the conference'
    echo 'server is recycled.'
  fi
  echo '<p><u><b>Refresh Interval</b></u>'
  echo '<p>For your convenience, the panel may be configured to refresh itself'
  echo "at certain time intervals, so you'll always have an up-to-date status"
  echo 'display.  Any commands or messages that you issue will reset the '
  echo 'automatic refresh interval back to "None" (which is the default when'
  echo 'you first load the utility).'
  echo '<p>To change the refresh interval, click the radio button corresponding'
  echo 'to your desired rate (for example, every 30 seconds) and click the'
  echo '"Set / Refresh" button.'
  echo '<p>You may click the "Set / Refresh" any time to refresh the information'
  echo 'in the panel, even when automatic refresh is disabled (set to "None").'
  echo '<div align=right><form>'
  echo '<input type=button value="Close Window"'
  echo 'onClick={self.close();}></form></div>'
fi
echo '<hr><font size="-1">&copy;2006, 2007 <i>Crafted by KC4YOZ</i><br>'
echo 'Distributed under the terms of the'
echo '<a href="http://www.gnu.org/licenses/gpl.html">GNU General Public License</a>.'
echo '</font>'

#  Debug only...
#echo '<pre>'
#set
#echo '</pre>'

echo '</body></html>'
exit 0
