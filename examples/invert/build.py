import subprocess

subprocess.run([
    "cl", "-nologo",
    "-Z7",
    "invert.c",
    "-I", "../../",
    "-link", "-out:InvertC.ofx", "-dll", "-incremental:no", "-debug",
])
