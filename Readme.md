Hektor is a ‘fair access policy’ tool for Hughesnet users — it lets you know the
usage remaining before the FAP is activated so you can use your Internets
wisely. In addition, once the FAP is activated, hektor tells you the exact time
it will be deactivated. Hektor is small and quick, with a simple command-line
interface:

    $ hektor
    140.42 megabytes are remaining.

---

    $ hektor
    The FAP will be deactivated in 19.32 hours, at 4:02 PM Monday.

## Installation

On Arch Linux, hektor can be installed through the AUR as the [hektor-git]
package. With a tool like [bauerbill], it’s as simple as:

    $ sudo bauerbill --aur -S hektor-git

[hektor-git]: http://aur.archlinux.org/packages.php?ID=37095
[bauerbill]: http://xyne.archlinux.ca/info/bauerbill

Bauerbill will automatically handle everything. On other Linux distros, hektor’s
only dependency, [libcurl], will need to be installed. On Ubuntu, the
[libcurl4-openssl-dev][ubuntu-libcurl] package accomplishes that:

    $ sudo apt-get install libcurl4-openssl-dev

[libcurl]: http://curl.haxx.se/libcurl/
[ubuntu-libcurl]: http://packages.ubuntu.com/lucid/libcurl4-openssl-dev

Once libcurl is installed, download and extract [hektor’s tarball][tarball].
Then, compile and install:

    $ wget http://github.com/kchmck/hektor/tarball/master
    [...]
    $ tar -vxzf kchmck-hektor-*.tar.gz
    [...]
    $ cd kchmck-hektor-*
    $ make
    $ sudo make install
    [...]

[tarball]: http://github.com/kchmck/hektor/tarball/master

The `hektor` command should then be installed to `/usr/bin/hektor` and be
available to use:

    $ hektor
    200.00 megabytes are remaining.
