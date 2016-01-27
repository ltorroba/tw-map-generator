# Tribal Wars Map Generator

This program generates a variety of maps given a Tribal Wars server and world number. It is integrated with the backend at maps.zes.me that provides up-to-date family and war data from contributing players.

## Installation

The program should be compiled using G++-5 and with the following libraries: libcurl, libcairo, libssl, libcrypto, libptread. The directory for cairo's header files should also be included using the -I flag.

## Usage

As off v1.0.0, the following arguments must be passed on execution.

-k [AWS key ID] -- Amazon Web Services key ID
-a [AWS secret access key] -- Amazon Web Server secret access key
-s [2 character TW server code] -- 2 character TW server code, in lowercase. Example: "br", "ru", "pl"
-w [world number] -- World number. Must be integer. Example: 23, 99, 104, 3
-m [metadata page URL] -- URL for metadata TW world metadata information. http://maps.zes.me/metadata.php works fine in most cases
-l (optional) [path (optional)] -- Save generated images locally. If no path is specified, they are saved at the executable's directory.
-u (optional) -- Upload images to AWS.

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
