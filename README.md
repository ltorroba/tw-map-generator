# Tribal Wars Map Generator

This program generates a variety of maps given a Tribal Wars server and world number. It is integrated with the backend at maps.zes.me (no longer running) that provides up-to-date family and war data from contributing players.

## About Tribal Wars and this project
[Tribal Wars](https://www.tribalwars.net/en-dk/) is a browser-based game I used to play at the time I wrote this project.
Each player starts out with a small village that generates some resources. They can also attack
other players to pillage their resources. They use these resources to improve their own village.
Eventually, through enough investment they may acquire the tools to conquer other people's villages.

This simple dynamic gives rise to an interesting community, as players band together into "tribes" which
cooperate to control parts of the world. These in turn form alliances and even families (separate tribes that
are operate under the same leadership), which fight other alliances/families.

Territory control is thus vital for these games. While tools existed to visualize this, I always thought that
their interfaces were a bit out-of-date. Eventually, lack of maintance broke some of these tools, which
players relied on to get weekly and monthly progress reports. I wanted to come up with a new and improved replacement. 

I was happy with the
end result of the project (see images below), but I did not forsee how expensive it would be to host these
images on an AWS server for other people to see (especially since I really wanted to have high-res images, unlike
former tools).
In the end, I had to stop the script as I was spending too
much money on it. Still, I have some images that I generated at the time below for sample purposes.

![Families map](https://raw.githubusercontent.com/ltorroba/tw-map-generator/master/images/top_families_hr.png "Families map")
![Top players by ODD (Opponents defeated as a defender)](https://raw.githubusercontent.com/ltorroba/tw-map-generator/master/images/top_players_odd_hr.png "Top players by ODD (Opponents defeated as a defender)")
![Top tribes map](https://raw.githubusercontent.com/ltorroba/tw-map-generator/master/images/top_tribes_hr.png "Top tribes map")

If anyone feels like actually setting this up, let me know. I would be happy to help.

## Installation

The program should be compiled using G++-5 and with the following libraries: libcurl, libcairo, libssl, libcrypto, libptread. The directory for cairo's header files should also be included using the -I flag.

## Usage

As of v1.0.0, the following arguments must be passed on execution:

- `-k [AWS key ID]` -- Amazon Web Services key ID
- `-a [AWS secret access key]` -- Amazon Web Server secret access key
- `-s [2 character TW server code]` -- 2 character TW server code, in lowercase. Example: "br", "ru", "pl"
- `-w [world number]` -- World number. Must be integer. Example: 23, 99, 104, 3
- `-m [metadata page URL]` -- URL for metadata TW world metadata information. http://maps.zes.me/metadata.php works fine in most cases
- `-l` (optional) [path (optional)] -- Save generated images locally. If no path is specified, they are saved at the executable's directory.
- `-u` (optional) -- Upload images to AWS.

## Contributing

1. Fork the repository
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request. Remember to reference any issues, if that is the case.

## History

This project began as an attempt for me to learn C++ in a fun way, and as such, you may see different conventions used, as well as minor problems. Please open issues to fix them, I am still learning! :D

## License

This program is licensed under a GNU GPL v3 license.
