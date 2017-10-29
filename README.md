# CuteMail
All mail clients suck. This one just sucks less. (c) circa (mutt)

WIP shell for fetchmail, mta and MHonArc, fast mail client.

### Why?
Because every mail clients sucks. Nuff said. 
- Mutt is good, but it's too terminalish. Yes, I like GUI, kill me for that. \*curses-like GUI is text-based interface, so I don't think we must determine it as *Graphical* User Interface.
- Thunderbird\IceDove sucks - they using Gecko technology, that almost dead. It's too complicated to be mail client.
- Geary - too Gnome-like. Future less mail client, seriously. I even can't use multiply accounts.
- Claws-mail is good enough, but with multiple accounts it's sucks. It can't even connect to my account and, sometimes, gets a removed mail. I would like to commit to their code, but I don't like GTK developing.
- Trojitá, KMail - nailed to KDE infrastructure, and it's pointless to use it outside of KDE.

Aaaaaaaand, that's all. So far I don't see any alternatives for GNU/Linux. So we're need to reinvent the bicycle.

### Goal
Creating:
- Mail client with multiple accaunts support.
- No browser, no javascript.
- Junk mail must be move to junk folders, without deleting, user must dicede what (s)he want to do with them.
- Clear and understandable interface - no useless buttons and settings. (hello, Thunderbird!)

### Dependencies
- Qt5
- Compiller with C++11 support
- Fetchmail
- Procmail
- MHonArc (may be depricated in future update)

MTA (e.g. Procmail) will be configured in future update. For example: default MTA in Debian (and maybe *buntu-like) is Exim, so we need to use it.

### LICENSE
See LICENSE in root repository.
