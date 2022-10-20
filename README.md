# eks-codeloader
A simple loader for hand-crafted seewa-eks assembly/machine code. See [seewa-eks](https://github.com/fxsheep/seewa-eks) for details.

## Usage 
Make sure to change `CC` in Makefile. Target device may have a decade old kernel, so please use a low gcc version.

### Command line arguments
```
./codeloader <addr> <codefile>
```
where
 - `addr` is address to load code to
 - `codefile` is a handcrafted list of instructions, see `examples/`

### Note
 - Do not have an empty newline after your instructions in `codefile`
  
