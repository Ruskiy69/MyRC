# MyRC #
A simple, cross-platform, command-line IRC client.

(only tested on Win7 and Ubuntu 9.10)

```
USAGE: ./MyRC [hostname] [port] [nick]

hostname    - An optional IRC server hostname to connect to. Defaults to irc.hackthissite.org
port        - An optional port to connect to. Defaults to 6667. SSL is currently NOT supported.
nick        - An optional nickname to use on the IRC server. Defaults to MyRC-User
```

If you only want a custom nick, with default hostname / port, use the syntax:

    MyRC.exe NULL NULL [your_nick]

Nicknames must not contain spaces or symbols, but underscores (`_`) are allowed.

