<p align="center">
 <img width="350" alt="so_long" src="https://github.com/tobeshota/pipex/assets/103044771/91b4ae97-c39e-4551-8375-68b7c15cfe4c">
</p>

**Pipex is one of the [42](https://42tokyo.jp/) projects.**  
**It is a reimplementation of pipes.**

---

# Description
A pipe is a mechanism for connecting I/O between processes.   
By reimplementing pipes, you can deepen your understanding of redirection and pipes.

# Usage
### Installation
Clone this repository.
```
git clone https://github.com/tobeshota/pipex.git
```
Change directory to this repository.
```
cd pipex
```
Compile to create an executable file: `pipex`.
```
make
```

### Execute
#### Handle multiple pipes
This:
```
./pipex file1 cmd1 cmd2 ... cmdn file2
```
behaves like:
```
< file1 cmd1 | cmd2 ... | cmdn > file2
```
#### Handle heredoc
This:
```
./pipex here_doc LIMITTER cmd1 cmd2 ... cmdn file2
```
behaves like:
```
<< LIMITTER cmd1 | cmd2 ... | cmdn >> file2
```
