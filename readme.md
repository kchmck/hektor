### Hektor

Hektor is a calculator for Hughesnet's ʻfair access policyʼ – it lets you know
the amount of megabytes remaining before the FAP is activated, so you can use
your Internets wisely. It is small and quick, with a simple command-line
interface:

    $ hektor
    102.03 megabytes remaining
    $ hektor record
    Recorded snapshot number 98.
    100.42 megabytes remaining

### Installing jansson

The [Jansson library] is used to save and load snapshots to and from the disk in
the [json format].

Users of **Arch Linux** can install the library with the [jansson
package][jansson-aur] from the AUR. Using [bauerbill], it's as simple as `sudo
bauerbill --aur -S jansson`.

Users of **Ubuntu** can [add][add-ppa] the [library author's PPA][jansson-ppa]
then run `sudo apt-get install jansson`.

**Others** can download and [compile][jansson-compile] [the latest
release][jansson-dl].

[Jansson library]: http://digip.org/jansson/
[json format]: http://json.org
[jansson-aur]: http://aur.archlinux.org/packages.php?ID=35622
[bauerbill]: http://xyne.archlinux.ca/info/bauerbill
[jansson-ppa]: https://edge.launchpad.net/~petri/+archive/ppa
[add-ppa]: https://edge.launchpad.net/+help/soyuz/ppa-sources-list.html
[jansson-dl]: http://digip.org/jansson/releases/jansson-1.2.1.tar.bz2
[jansson-compile]: http://digip.org/jansson/doc/1.2/gettingstarted.html

### Installing cURL

[cURL] is used to fetch pages and usage information from the modem.

**Arch Linux** users can install the `curl` package from the extra repos: `sudo
pacman -S curl`.

**Debian** or **Ubuntu** users can install the `libcurl4-openssl-dev` package:
`sudo apt-get install libcurl4-openssl-dev`.

For **Others**, the library should normally be available from your package
manager of choice with a name resembling `curl` or `libcurl`. Otherwise,
download and compile [the latest release][curl-dl].

[cURL]: http://curl.haxx.se/
[curl-dl]: http://curl.haxx.se/download.html

### Installing xdg-basedir

Hektor uses the xdg-basedir library in order to store configuration and
snapshots in predictable and [standardized] locations.

Users of **Arch Linux** can install the `libxdg-basedir` package from the
community repos: `sudo pacman -S libxdg-basedir`.

Users of **Debian** or **Ubuntu** can install the `libxdg-basedir-dev` package:
`sudo apt-get install libxdg-basedir-dev`.

**Others** can download and compile [the source][basedir-dl].

[standardized]: http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html
[basedir-dl]: http://n.ethz.ch/student/nevillm/download/libxdg-basedir/

### Installing lua

[lua] is used for hektor's configuration file.

[lua]: http://lua.org/

### Installing hektor

### Configuring

Hektor has but one little configuration option: a usage plan. The configuration
file is ususally stored at `~/.config/hektor/config.lua` and can be edited with
you favorite text editor.

### Using

### Hacking
