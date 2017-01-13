# FCl: Windows MITM proxy for proprietary miners
This software written and originally compiled under Embarcadero RAD Studio XE2 C++ Builder, but probably may compiled and with newer versions of RAD Studio. For more early versions - you may have problems with UnicodeString type, as it was added only in XE2. If you using more early versions of RAD Studio, try replace UnicodeString types with AnsiString - it may help.
Also software uses Overbyte ICS v.8.16 network components, so you you will need it too. You can get it here - http://www.overbyte.be/frame_index.html

# Usage
Software has simple and friendly interface. Just set:
<ul>
<li><b>Local port</b> - port on your PC, which programm will listen</li>
<li><b>Remote port</b> - target port of remote server</li>
<li><b>Domain address and IP address</b> - domain name and IP of target server</li>
<li><b>Replace worker/wallet</b> - your username on pool: usually your wallet or pair login.worker</li>
<li><b>Miner version</b> - select miner, which you use</li>
<li><b>Log</b> - select logging level which will be used</li>
</ul>
More details on <a href="https://bitcointalk.org/index.php?topic=1734152.0">bitcointalk</a>

# Thanks
This software uses and/or based next components/source code, and I very grateful to these people for their work (in chronological order):
<ol>
<li>Overbyte ICS network component suite, by Francois PIETTE, francois.piette@overbyte.be</li>
<li>"Socketspy" program, written on Object Pascal by wilfried@mestdagh.biz</li>
<li>Some modifications and main idea from person, who doesnt want disclose him name/contacts...but, here is his bitcoin wallet - 12vAKsTVuchU9qU6GEGDUvEZ5AE1e4jEMn</li>
</ol>
