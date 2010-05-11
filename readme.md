# Hektor

Hektor is a calculator for Hughesnet’s ‘[fair access policy]’ — it lets you know
the amount of megabytes remaining before the FAP is activated, so you can use
your Internets wisely. It is small and quick, with a simple command-line
interface:

    $ hektor
    102.03 megabytes are remaining.
    $ hektor record
    Snapshot #98 was recorded.
    100.42 megabytes are remaining.

Hektor works through the use of periodically recorded ‘snapshots’. Each snapshot
contains the time it was recorded, along with upload and download numbers
extracted from the modem. Using a list of these snapshots, hektor can calculate
a (rough) rolling usage estimate based on the amount of data transferred and
refilled between each recording. The following sections describe how to get
hektor up and running on your system.

[fair access policy]: http://customercare.myhughesnet.com/fap_faqs.htm

## Installing hektor

On [arch linux], hektor can be installed from the AUR as the [hektor-git]
package. Using a tool like [bauerbill], it’s as simple as:

    $ sudo bauerbill --aur -S hektor-git

Bauerbill will automatically install all of hektor’s dependencies. On other
distros, a few libraries must be installed before hektor can be built;
instructions for doing so are below.

[arch linux]: http://archlinux.org/
[hektor-git]: http://aur.archlinux.org/packages.php?ID=37095
[bauerbill]: http://xyne.archlinux.ca/info/bauerbill

## Installing hektor’s dependencies

- The [Jansson library] is used to save and load snapshots to and from the disk in
  the [json format]. **Users of Ubuntu** can add the [library author’s
  PPA][jansson-ppa] then run `sudo apt-get install jansson`. **Others** can
  download and [compile][jansson-compile] [the latest release][jansson-dl].

[Jansson library]: http://digip.org/jansson/
[json format]: http://json.org
[jansson-ppa]: https://edge.launchpad.net/~petri/+archive/ppa
[jansson-dl]: http://digip.org/jansson/releases/jansson-1.2.1.tar.bz2
[jansson-compile]: http://digip.org/jansson/doc/1.2/gettingstarted.html

- [cURL] is used to fetch pages and usage information from the modem. **Ubuntu
  users** can install the `libcurl4-openssl-dev` package: `sudo apt-get install
  libcurl4-openssl-dev`. For **Others**, the library should be available from
  your package manager of choice with a name resembling `curl` or `libcurl`;
  otherwise, download and compile [the latest release][curl-dl].

[cURL]: http://curl.haxx.se/
[curl-dl]: http://curl.haxx.se/download.html

- The xdg-basedir library is used to store configuration and snapshots in a
  predictable and [standardized] location. **Users of Ubuntu** can install the
  `libxdg-basedir-dev` package: `sudo apt-get install libxdg-basedir-dev`.
  **Others** can download and compile [the source][basedir-dl].

[standardized]: http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html
[basedir-dl]: http://n.ethz.ch/student/nevillm/download/libxdg-basedir/

- [lua] is used for hektor’s configuration file. **Users of Ubuntu** can install
  the `liblua5.1-0-dev` package: `sudo apt-get install liblua5.1-0-dev`.
  **Others** can install it from their respective package manager or by manually
  compiling [the latest release][lua-dl].

[lua]: http://lua.org/
[lua-dl]: http://www.lua.org/ftp/lua-5.1.4.tar.gz

## Building and installing hektor

After installing all of hektor’s dependencies, download and extract [the
tarball][hektor-dl]. Then, change into the extracted folder, build, and install:

    $ wget http://github.com/kchmck/hektor/tarball/master
    [...]
    $ tar -zvxf kchmck-hektor-*.tar.gz
    [...]
    $ cd kchmck-hektor-*
    $ make
    [...]
    $ sudo make install
    [..]

Once installed, the `hektor` command should be available to use!

    $ hektor
    200.00 megabytes are remaining.

[hektor-dl]: http://github.com/kchmck/hektor/tarball/master

## Configuring hektor

Hektor has just one little configuration option: a usage plan. The configuration
file is usually stored at `~/.config/hektor/config.lua`, and can be edited with
your favorite text editor. After hektor’s first run, the config file should
contain:

    -- Your Hughesnet usage plan, which can be one of: home, pro, pro-plus,
    -- elite, elite-plus, or elite-premium.
    usage_plan = "home"

As it says, the `usage_plan` option can be set to `home`, `pro`, `pro-plus`,
`elite`, `elite-plus`, or `elite-premium`, based on your Hughesnet plan. It
defaults to the `home` plan.
