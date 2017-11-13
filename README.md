# FCl: Windows MITM proxy for proprietary miners
This software written and originally compiled under Embarcadero RAD Studio XE2 C++ Builder.

# Usage/build dependencies
<ul>
<li><b>Overbyte ICS network components</b> - v.8.16, TSslWSocketClient and TSslWSocketServer, http://www.overbyte.be/frame_index.html</li>
<li><b>WinDivert driver components</b> - v.1.1.8, WinDivert64.sys and WinDivert.dll , https://github.com/basil00/Divert/releases</li>
<li><b>OpenSSL</b> - v.1.0.1j or compatible, libeay32.dll and ssleay32.dll, https://www.openssl.org/</li>
<li><b>Related dll's</b> - libwinpthread-1.dll, libgcc_s_dw2-1.dll</li>
<li><b>SSL certificates</b> - rootCA.crt, FCl.key and FCl.crt for each minig pool(flypool, nanopool, suprnova), placed in ./_poolname_/ dir</li>
</ul>

# Usage
More details on <a href="https://bitcointalk.org/index.php?topic=1734152.0">bitcointalk</a>

# Thanks
This software uses and/or based next components/source code, and I very grateful to these people for their work (in chronological order):
<ol>
<li>Overbyte ICS network component suite, by Francois PIETTE, francois.piette@overbyte.be</li>
<li>"Socketspy" program, written on Object Pascal by wilfried@mestdagh.biz</li>
<li>Some modifications and main idea from person, who doesnt want disclose him name/contacts...but, here is his bitcoin wallet - 12vAKsTVuchU9qU6GEGDUvEZ5AE1e4jEMn</li>
<li>Divert driver for capturing/modifying/reinjecting packets, by basil00, https://github.com/basil00/Divert</li>
</ol>
