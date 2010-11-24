Hektor is a fast and simple 'fair access policy' tool for Hughesnet users.
Through [lua] hooks, hektor can be setup to prevent FAP activation, to pop up
notifications, or to perform an unlimited number of other actions.

    $ hektor
    30.0 megabytes are remaining

The following sections detail how to install and configure hektor.

### Installing

On [arch linux], hektor can be installed through the AUR as the [hektor-git]
package. With a tool like [bauerbill], it's as simple as:

    $ sudo bauerbill --aur -S hektor-git

[arch linux]: http://archlinux.org/
[hektor-git]: http://aur.archlinux.org/packages.php?ID=37095
[bauerbill]: http://xyne.archlinux.ca/info/bauerbill

Bauerbill will automatically install all of hektor's dependencies. On other
Linux distros, three libraries must be installed before hektor can be built.
Instructions for doing so are below.

### Installing Dependencies

- [cURL] is used to fetch pages and usage information from the modem. **Ubuntu
  users** can install the `libcurl4-openssl-dev` package: `sudo apt-get install
  libcurl4-openssl-dev`. For **Others**, the library should be available from
  one's package manager of choice with a name resembling `curl` or `libcurl`.
  Otherwise, download and compile [the latest release][curl-dl].

[cURL]: http://curl.haxx.se/
[curl-dl]: http://curl.haxx.se/download.html

- The xdg-basedir library is used to store configuration in a predictable and
  [standardized] location. **Users of Ubuntu** can install the
  `libxdg-basedir-dev` package: `sudo apt-get install libxdg-basedir-dev`.
  **Others** can download and compile [the source][basedir-dl].

[standardized]: http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html
[basedir-dl]: http://n.ethz.ch/student/nevillm/download/libxdg-basedir/

- [lua] is used for hektor's configuration file. **Users of Ubuntu** can install
  the `liblua5.1-0-dev` package: `sudo apt-get install liblua5.1-0-dev`.
  **Others** can install it from their respective package manager or by manually
  compiling [the latest release][lua-dl].

[lua]: http://lua.org/
[lua-dl]: http://www.lua.org/ftp/lua-5.1.4.tar.gz

### Building and Installing

After installing all of hektor's dependencies, download and extract [the
tarball][hektor-dl]. Then, change into the extracted folder, build, and install:

    $ wget http://github.com/kchmck/hektor/tarball/master
    [...]
    $ tar -vxzf kchmck-hektor-*.tar.gz
    [...]
    $ cd kchmck-hektor-*
    $ make
    [...]
    $ sudo make install
    [...]

[hektor-dl]: http://github.com/kchmck/hektor/tarball/master

Once installed, the `hektor` command should be available to use:

    $ hektor
    200.0 megabytes are remaining

### Configuring Hooks

Hektor's default configuration is pretty bare bones: it simply prints out the
important information and exits. Through its configuration file, though, hektor
can be setup with virtually unlimited flexibility.

The configuration file is usually located at `~/.config/hektor/config.lua`. As
noted by the extension, the file is simply a [lua script] and can be edited with
one's favorite text editor. After hektor's first run, the file should contain:

[lua script]: http://lua.org/pil/index.html

    when_fap_is_inactive(function (hektor)

      print(hektor.remaining_string .. " are remaining")

      -- if hektor.remaining_mb < 5 then
      --   hektor.restart_modem()
      -- end

    end)

    when_fap_is_active(function (hektor)

      print(hektor.refill_string .. " until FAP deactivation (at " ..
            os.date("%I:%M %p %A", hektor.refill_timestamp) ..  ")")

    end)

Hektor uses two hooks, depending on the FAP status:

#### when_fap_is_active(hook)

This hook is ran when the FAP is active.

#### when_fap_is_inactive(hook)

This hook is ran when the FAP is not active.

Both hooks receive the "hektor table", detailed below, as their only argument,
and can use it to perform all kinds of logic, from simply printing out the
information:

    when_fap_is_inactive(function (hektor)
      print(hektor.remaining_string .. " are remaining")
    end)

to popping up a notification (with the help of [lua's `os.execute`
function][os.execute]):

    when_fap_is_inactive(function (hektor)
      if hektor.remaining_usage < 10 then
        os.execute("notify-send -u critical " ..
                   "'Remaining usage is dangerously low' " ..
                   "'Only " .. hektor.remaining_string .. " are remaining'")
      end
    end)

[os.execute]: http://lua.org/manual/5.1/manual.html#pdf-os.execute

to restarting the modem (this works especially well when hektor is run every 15
seconds or so, as it effectively prevents any kind of internet activity):

    when_fap_is_inactive(function (hektor)
      if hektor.remaining_usage < 5 then
        hektor.restart_modem()
      end
    end)

The above methods could all be combined, as well:

    when_fap_is_inactive(function (hektor)
      print(hektor.remaining_string .. " are remaining")

      if hektor.remaining_usage < 10 then
        os.execute("notify-send -u critical " ..
                   "'Remaining usage is dangerously low' " ..
                   "'Only " .. hektor.remaining_string .. " are remaining'")
      end

      if hektor.remaining_usage < 5 then
        hektor.restart_modem()
      end
    end)

### The Hektor Table

The hektor table is passed to both the hooks. It provides access to various
modem information and functions, as detailed below.

#### allowed_usage (number)

This field is the usage allowed by Hughesnet, based on one's usage plan. On
HN7000 and similar modems, this field is represented in megabytes, and on HN9000
modems it is represented in [mebibytes].

[mebibytes]: http://en.wikipedia.org/wiki/Mebibyte

For example, with an HN7000S and a home plan:

    hektor.allowed_usage == 200

and with an HN9000 and a home plan:

    hektor.allowed_usage == 250

#### allowed_string (string)

This field is a helpful string representation of the allowed usage. With the
sames setups above, it will resemble:

    hektor.allowed_string == "200.0 megabytes"

and

    hektor.allowed_string == "250.0 mebibytes"

#### remaining_usage (number)

This field provides the amount of usage remaining before the FAP is activated.
As with the allowed field, this is represented as megabytes on HN7000 modems and
as mebibytes on HN9000 modems. It can be used to prevent FAP activation when a
lower threshold is reached:

    -- Check if remaining usage is less than 10 megabytes/mebibytes
    if hektor.remaining_usage < 10 then
      -- Take some action here
    end

    if hektor.remaining_usage > 15 then
      -- Reverse the actions taken above
    end

#### remaining_pct (number)

This is the percentage of remaining usage. It is simply `allowed_usage /
remaining_usage * 100`, and will be a floating-point number between 0 and 100.

#### remaining_string (string)

As with `allowed_string`, this is a helpful string representation of
`remaining_usage`. For example:

    print(hektor.remaining_string)

could print something like the following, with an HN7000S modem:

    30.2 megabytes

#### refill_seconds (number)

This field is the number of seconds remaining before the FAP is deactivated, or,
on HN9000 modems, the number of seconds remaining before the daily refill time.

#### refill_timestamp (unix timestamp)

This is a [Unix timestamp] of the (almost) exact time of FAP deactivation or the
daily refill. It can be used with [lua's `os.date` function][os.date] to create
a human-readable date:

    print(os.date("%c", hektor.refill_timestamp))

could print something like:

    Thu Jul  8 18:49:23 2010

As noted in the lua documentation, the `os.date` function takes the same
arguments as [C's `strftime` function][strftime].

[Unix timestamp]: http://en.wikipedia.org/wiki/Unix_timestamp
[os.date]: http://lua.org/manual/5.1/manual.html#pdf-os.date
[strftime]: http://cplusplus.com/reference/clibrary/ctime/strftime/

#### refill_string (string)

This is a helpful string representation of `refill_time`. For example:

    print(hektor.refill_string)

could print something like:

    9.2 hours

or:

    30.0 minutes

#### restart_modem() (function)

This function restarts the modem and returns `nil`. Its usage is simple:

    hektor.restart_modem()
