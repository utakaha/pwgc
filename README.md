# pwgc: A Password Generator
A password generator written in C.

## Usage

```bash
# Generate a 20-character password (default)
$ pwgc
=> ?9a)(6_$G?x..yqWC3+2

# Generate a password with custom length (1-100 characters)
$ pwgc -l 30
=> c=oU6V=f6ckr4Y3WDBb%]6u+hNrT4G

# Generate alphabetic only password
$ pwgc -a
=> RpCbztADEZxSQumjbmIm

# Generate alphanumeric password (no symbols)
$ pwgc -n
=> mV6Pm40d8EsgxwkTsfdT

# Generate password with letters and symbols (no numbers)
$ pwgc -s
=> yiC(pUJRpGrgIvw,$NGx
```

## Compile
```bash
make
```

## Test
```bash
make test
```

## Installation
Install to `/usr/local/bin`:
```bash
sudo make install
```

Or install to user directory (no sudo required):
```bash
make PREFIX=$HOME/.local install
```
