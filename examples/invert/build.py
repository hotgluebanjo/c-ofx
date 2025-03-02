import subprocess

subprocess.run([
    "cl",
    "invert.c",
    "-I", "../../",
    "/link", "/out:Invert.ofx", "/dll",
])
