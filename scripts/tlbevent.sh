#!/bin/bash
#
# event script invoked by tlb events to link connections 
# and do other useful things you may wish to add.
#
# To use this script and the ".link" feature
# of theLinkbox, make these settings in your tlb.conf file:
#
#   ConfEnable = 0     ; disable conferencing
#   EventScript = /home/tlb/scripts/tlbevents.sh  ; path to this script
#
# Unless modified, this script will log useful information 
# to /home/tlb/log/messages
#
# There are many other things you can do from this script baesed
# on tlb events.  See theLinkbox SCRIPTING documentation.
#
# Note: be sure connecting nodes use unique callsigns/usernames.
#
# WD5M
#
TLBCMD="/usr/local/bin/tlbcmd"    # path to "tlbcmd" executable
# Logging
function log {
        local LOGFILE=/home/tlb/log/messages
        local MESSAGE="TLB: $@"
        echo "${0##*/}:${MESSAGE}"
        if [[ -n "${LOGFILE}" ]]
	then
                echo "`date '+%b %d %Y %T %z'` ${MESSAGE}" >> $LOGFILE
        fi
}
# The linkit function loops through connected users and links new connection
function linkit {
	THISCALL="${1}"
	USERS=$(${TLBCMD} -s "..users")
	# Is ConfEnable enabled?
	ISCONF=$(${TLBCMD} -s "..set ConfEnable"|tail -1|cut -d' ' -f3)
	if [[ ${ISCONF} -eq 1 ]]
	then
		log "Linking is not supported with ConfEnable enabled."
		log "Setting ConfEnable = 0"
		JUNK=$(${TLBCMD} -s "..set ConfEnable = 0")
	fi
	IFS=$'\n'
	for line in ${USERS}
	do
		unset IFS
		set ${line}
		if [[ "${1}" == *[0-9]. ]]
		then	# this is a user record
			if [[ "${3}" == *K* ]]||[[ "${3}" == *x* ]]; then   # Kicked
				log "${2} is allready kicked or inactive, skipping link: ${*}"
				continue        # Already kicked or inactive
			fi
			if [[ "${2}" == "${THISCALL}" ]]; then
				log "${THISCALL} skip same node: ${*}"
				continue        # skip same node
			fi
			if [[ "${3}" == *R* ]]; then   # Monitor enabled
				if [[ ${MONITOR} -eq 1 ]]
				then # unlink when both enable monitor
					log "unlink ${THISCALL} ${2} in monitor mode: ${*}"
					JUNK=$(${TLBCMD} -s "..unlink ${THISCALL} ${2}")
				else
					log "link -m ${THISCALL} ${2}: ${*}"
					JUNK=$(${TLBCMD} -s "..link -m ${THISCALL} ${2}")
				fi
			else
				log "link ${THISCALL} ${2}: ${*}"
				JUNK=$(${TLBCMD} -s "..link ${THISCALL} ${2}")
			fi
		fi
	done
} # end of linkit function
#
# Do something based on event arguments.
# ${1} = <event> see SCRIPTING.txt in tlb source.
# ${2} = <type> one of "echolink" "speakfreely" "rtp" "outbound"
# ${3} = <user>
# ${4} = <user_count>
#
log "${1} ${2} ${3} ${4}"
if [[ "${1}" = "connected" ]]
then
	linkit "${3}"	# link to other ports
fi
exit 0
