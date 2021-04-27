# Commaxian
The final project for CSC161: Computer Science 2. The objective is to program an implementation of a video game. My submission for this project was originally intended to be a command-line implementation of Galaxian for Windows.

After researching an issue with multiple calls to cout failing after a while, an alternative windows-only method was researched.
The windows-only method altered individual character cells from the command line, but still required a full half-second to update a small console window.

After these failures, it was decided to learn how to use the Simple Fast Multimedia Library (SFML) for a graphical rendition of the game. This change has proven to be a major success, especially since SFML provides many objects, processes, and bits of code that vastly improve the development experience. This makes it easier to program a game without having to worry about a lot of the underlying code for things such as creating a graphical window or sending shaders the the GPU.
