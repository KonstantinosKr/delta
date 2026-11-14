# Report and video tooling

Regenerates `output/report/hopper_bug_report.pdf` and `output/report/hopper-fall.mp4` from a real
run of the funnel test. Not built or run by ctest; the numbers in the report are
asserted by `tests/hopper_test.cpp` and `tests/hopper_fall_test.cpp`.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

./build/hf_video output/report          # trajectory CSVs (600 frames)
python3 tools/report/make_report.py     # figure PNGs
python3 tools/report/render_video.py    # frames/frame_0000.png ...
python3 tools/report/make_pdf.py        # hopper_bug_report.pdf

ffmpeg -framerate 40 -i output/report/frames/frame_%04d.png \
       -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" \
       -c:v libx264 -crf 20 -pix_fmt yuv420p -movflags +faststart \
       output/report/hopper-fall.mp4
```

## The 15 651-body hopper

```sh
./build/hf_big output/big 12000 300 1e-5   # steps, frames, dt
python3 tools/report/render_big.py         # output/big/frames_big/*.png
ffmpeg -framerate 25 -i output/big/frames_big/frame_%04d.png \
       -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" \
       -c:v libx264 -crf 20 -pix_fmt yuv420p -movflags +faststart \
       output/big/hopper-big.mp4
```

`hf_big` prints the wall-clock cost as it goes and the totals at the end.

| file | role |
|---|---|
| `hf_video.cpp` | runs the funnel scene, writes `video_particles.csv`, `video_walls.csv`, `hopper_after.csv` |
| `make_report.py` | the five report figures (matplotlib) |
| `render_video.py` | orthographic painter's-algorithm renderer, one PNG per frame |
| `make_pdf.py` | the PDF itself (reportlab); needs the PNGs from `make_report.py` |
| `hf_big.cpp` | runs `hopper.mbfcp` (15 651 bodies) and its timings |
| `render_big.py` | renders the big run (15 625 spheres per frame, scatter) |

Everything lands in `output/report/` unless `DELTA_REPORT_DIR` says otherwise.
`make_pdf.py` writes the PDF there; copy it to `docs/` to publish.
