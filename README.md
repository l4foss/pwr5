## Description
WIP Another power management for GNU/Linux

**(everthing is still in progress, so don't expect it to work properly)**

* helps you to manage your power, lasting your battery life
with an easy, beautiful default CLI interface.
* inspired by TLP so it comes with good default configuration and highly 
customizability in mind.

## Why another?
TLP was pretty good and useful, but there are still some problems with it. First, 
application was slow since it's written in Perl and Bash, furthermore
the design of TLP doesn't allow it to be used as a library for other applications
easily, for example, a GUI.

Some pwr5's pros over TLP:
* pwr5 is made with extensibility in mind, it provides a universal C library
for any application to use, either a cli, ncurses or a Gtk+ client.
* pwr5 is written in C99 so it's very fast, much faster than bash and perl so every time
you type a command, you don't have to wait.
* pwr5 tries to use `sysfs` interface and available library instead of invoking
directly the binary, which is slow and unstable
* by cutting off some overheads over Perl and Bash scripts, pwr5 is more power efficient.
A program which manages power has to be power efficient itself, of course.

## Build and run
Make sure that you have a working GCC that supports C99 
(4.8 or newer would be ok) on a recent kernel (3.19 or newer
should be good)

## Implemented features
- [x] Battery
- [ ] CPU
- [ ] Disk
- [ ] Graphics
- [ ] PCI
- [ ] USB
- [ ] Wireless
- [x] Backlight
- [ ] Config parser
- [ ] Commandline interface
- [ ] Systemd services

## Contributing
pwr5 needs your help to be a universal tool that supports many laptops.
1. Fork it
2. Create a new branch
3. Commit your changes to the new branch
4. Make a pull request

## License
pwr5 is published under MIT License

## Author(s)
* l4 (l4.foss@gmail.com)
