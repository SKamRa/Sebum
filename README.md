# Introduction

Sebum is a Stealer Malware that will fetch all the desired infos and paste it on a pastebin.
It will provide infos like OS, location, hostname, list of users, main folders content, Browser Cache, Browser History, Browser Cookies, Autofill infos, clipboard datas, screenshot (in form a encrypted hexadecimal), Processes Discovery...

It is fully stealthy and encrypted so it is not detected by common AV. 
It won't allow persistence or Pivilege escalation yet.

# Informations

Now, Sebum works only on Unix based system.

# Disclaimer

**Sebum** is a demonstration tool developed for educational and ethical purposes only.
This software is intended to be used strictly in controlled, safe, and legal environments. By downloading or using this project, you acknowledge and agree to the following terms:

- **Responsible Use:** You agree to use "Sebum" solely for ethical and educational purposes, such as research, cybersecurity education, and awareness training. It must never be used to exploit, harm, or interfere with any system, network, or data that you do not have explicit permission to access.

- **No Malicious Intent:** You are prohibited from using "Sebum" to conduct any unauthorized or malicious activities, including but not limited to hacking, data theft, or service disruption. Any harmful or illegal use of this software is the sole responsibility of the user, and the author will not be held liable for any damages or consequences resulting from misuse.

- **Compliance with Laws:** You must ensure that your use of "Sebum" complies with all applicable laws and regulations in your jurisdiction. The author does not endorse or encourage any illegal activities.

- **No Warranty:** "Sebum" is provided "as-is" with no warranties, express or implied. The author is not responsible for any unintended consequences, including damage to systems, data, or infrastructure that may result from the use of this software.

By proceeding with the use of this tool, you agree to take full responsibility for your actions and acknowledge that you will use **Sebum** only in a responsible, ethical, and legal manner.

# Ressources
## To do list [SEBUM MALWARE]
- [X] Get OS infos
    - [X] Os Family
        - [X] Linux
        - [X] Windows
        - [X] FreeBSD
        - [X] Mac OSX
        - [X] Android
        - [X] OpenBSD
    - [X] Os Name
    - [X] Architecture
        - [X] ARM
        - [X] AMD
        - [X] Intel
    - [X] Get target's hostname
    - [X] Get list of users

- [X] Get user's infos
    - [X] Get Clipboard content
    - [X] Get main folders content (/home; /opt; /var/www/html; /tmp)
    - [X] Get target's location
    - [] Screenshot of actual window (feature for v2)
    - [] Screenshot of camera (feature for v2)

- [] Get Bowser's infos
    - [] Get Borwser's cache
    - [] Get Browser's history
    - [] Get Browser's cookies
    - [] Get Browser's Autofill infos


- [] Exfiltration
    - [X] Configure exfiltration function
    - [] Configure multiple (5) Discord WebHook for data recovery


- [] Botnet
    - [] Implement choice to either compile with auto destruction or to create a botnet zombie
    - [] Create the C2C
    - [] Add "Launch on Startup" feature -> crontab?
    - [] Capable of getting update (if useful)
    

- [] Post exploitation
    - [] Excrypt data to send
    - [] Excrypt source code
    - [] Auto destruction
    - [] Code polymorphism methods

- [] Misc
    - [] Organize the F* code
    - [X] Look to include single header which include all headers itself
    - [] Interface
    - [] Code the disclaimer part (see XOSINT)
    - [] Make the banner

## To do list [SEBUM WEBSITE]
- [] Create the website
- [] ...
