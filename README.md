# CuteMail
All mail clients suck. This one just sucks less. (c) circa (mutt)

Yet another GUI email client that sucks less.

## Contents

* [Screenshots](#screenshots)
* [Why not using the web?](#why-not-using-the-web)
* [Goal](#goal)
* [Dependencies](#dependencies)
* [License](#license)

### Screenshots

<p align="center">
  <img src="screenshots/cutemail-main.png?raw=true" alt="Main screenshot"/>
  <img src="screenshots/cutemail-textmail.png?raw=true" width="250px" height="140px" alt="Main window with text"/>
  <img src="screenshots/cutemail-htmlmail.png?raw=true" width="250px" height="140px" alt="Main window with html"/>
</p>

### Why not using the web?
Because web is even more sucks, then every email client. Nuff said. And because I don't want to launch browser for just checking mail, I have many mail accounts, and I don't want use google to rule them all (TM).
- Mutt is good, but it's too terminalish. Yes, I like GUI, kill me for that. \*curses-like GUI is text-based interface, so I don't think we must determine it as *Graphical* User Interface.
- Thunderbird\IceDove sucks - they using Gecko technology, that almost dead. It's too complicated to be mail client.
- Geary - too Gnome-like. Future less mail client, seriously. I even can't use multiply accounts.
- Claws-mail is good enough, but with multiple accounts it's sucks. It can't even connect to my account and, sometimes, gets a removed mail. I would like to commit to their code, but I don't like GTK developing.
- Trojitá, KMail - nailed to KDE infrastructure, and it's pointless to use it outside of KDE.

Aaaaaaaand, that's all. So far I don't see any alternatives for GNU/Linux. So we need to reinvent the bicycle.

### Goal
Creating:
- Mail client with multiple accounts support.
- No browser (well, I already broke this rule, most mail depend on freaking browser, but I will isolate it as best as possible).
- No javascript.
- No calendar and this kind of things. Never understend, why using calendar in email client?
- Junk mail must be moved to junk folders, without deleting, user must dicede what (s)he want to do with them.
- Clear and understandable interface - no useless buttons and settings. (hello, Thunderbird!)

### Dependencies
- Qt5
- Compiller with C++11 support
- libetpan
- libmimetic (at this moment used like 1% of the futures, because it was simplier to write own parser)

### LICENSE
See LICENSE in root repository.
