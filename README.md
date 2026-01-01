# Fourier Animations

This project is me trying to implement what the Great Grant Sanderson from 3Blue1Brown did in his explanation of the Fourier series. Go check it out ![here](https://www.youtube.com/watch?v=r6sGWTCMz2k) if you haven't seen it before.

The only difference is I used C++ and openGL instead of python and manim (which in hindsight would have been much easier 🥲).
And yes, learning openGL was ... FUN🫠🫠.

Also instead of using Fourier series, FFT was used to calcualte the Discrete fourier transform.

---

## 🎥 Demo
![butterfly](https://github.com/user-attachments/assets/4ecae8d3-7dee-49e8-9198-69d4a9b95201)

## Controls

- **Scroll** — Zoom in / out  
- **W / A / S / D** — Pan the view  
- **0** — Halve the number of samples  
- **9** — Double the number of samples  
- **B** — Slow down the animation  
- **N** — Speed up the animation  

## Usage

```bash
sh ./buildNrun.sh "path/to/your.svg"
```

## Example

```bash
sh ./buildNrun.sh ./Input/butterfly.svg
```
