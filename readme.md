### Hektor

### Requirements

Various libraries are required for hektor to work. Read the following sections
for their installation instructions.

#### Jansson

The [Jansson library] is used to save and load snapshots to and from the disk in
the [json format]. It can be installed in the following ways:

- **Arch Linux**: from the AUR as the [jansson][jansson-aur] package. With
  [bauerbill], etc., it's as simple as `sudo bauerbill --aur -S jansson`.

- **Ubuntu**: from the library author's [PPA][jansson-ppa] as the `jansson`
  package. After [adding the PPA][add-ppa], run `sudo apt-get install jansson`
  to install the library.

- **Others**: by [downloading][jansson-dl] and [compiling][jansson-compile] the
  latest release.

[Jansson library]: http://digip.org/jansson/
[json format]: http://json.org
[jansson-aur]: http://aur.archlinux.org/packages.php?ID=35622
[bauerbill]: http://xyne.archlinux.ca/info/bauerbill
[jansson-ppa]: https://edge.launchpad.net/~petri/+archive/ppa
[add-ppa]: https://edge.launchpad.net/+help/soyuz/ppa-sources-list.html
[jansson-dl]: http://digip.org/jansson/releases/jansson-1.2.1.tar.bz2
[jansson-compile]: http://digip.org/jansson/doc/1.2/gettingstarted.html

#### cURL

[cURL] is used to fetch pages and usage information from the modem. It can be
installed like:

- **Arch Linux**: as the `curl` package from the extra repos: `sudo pacman -S
  curl`.

- **Debian** and **Ubuntu**: as the `libcurl4-openssl-dev` package: `sudo
  apt-get install libcurl4-openssl-dev`.

- **Others**: the library should usually be available from your package manager
  of choice with a name resembling `curl` or `libcurl`. Otherwise,
  [download][curl-dl] and compile the latest release.

[cURL]: http://curl.haxx.se/
[curl-dl]: http://curl.haxx.se/download.html

#### xdg-basedir

Hektor uses the xdg-basedir library in order to store snapshots in a predictable
and [standardized] location. It can be installed in various ways:

- **Arch Linux**: with the `libxdg-basedir` package from the community
  repos: `sudo pacman -S libxdg-basedir`.

- **Debian** and **Ubuntu**: with the `libxdg-basedir-dev` package: `sudo
  apt-get install libxdg-basedir-dev`.

- **Others**: by [downloading][basedir-dl] and compiling the source.

[xdg-basedir]: http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html
[basedir-dl]: http://n.ethz.ch/student/nevillm/download/libxdg-basedir/

### Configuration

### Installation

### Usage

### Hacking
