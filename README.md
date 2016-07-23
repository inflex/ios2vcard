## iOS2vCard

Coverts iOS Addresses database to vCard

### Features

- Generates vCard v3.0 output of all addresses in single call


### TODO

- Cover multiple iOS Address book formats
- Add more generation options


### Prerequisites

For Ubuntu developers, you'll need the following packages at a minium;

	$ apt-get install build-essential sqlite3-dev

### Installation

1. Clone the project

    $ git clone 'https://github.com/inflex/ios2vcard'

2. Build it

    $ make

3. Run it!

	$ ./ios2vcard addresses.sqlite3 > addressbook.vcf

