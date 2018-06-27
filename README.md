# osd_clock

A clock which uses the X On-Screen Display library for Linux OS desktops

Usage example:

`osd_clock --top --center --font "-*-nimbus mono-*-*-*-*-47-*-*-*-*-*-*-*"`

The default font is freemono. To correctly choose the desired font, it's recomendable to use the  
application **xfontsel**:
1. Execute the command: `xfontsel -print`
2. Choose an available font
3. Press the *quit* button, you'll get the name of the font at the terminal console. Don't forget  
quoting the name of the font if it has spaces.
4. Include the name of this font when executing osd_clock

---
To compile, type 'make'.  
To install, type 'make install'.  

For help, osd_clock -h.  Simple enough.

enjoy!


jon beckham <leftorium@leftorium.net>
