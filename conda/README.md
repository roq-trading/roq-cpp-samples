```bash
mamba install -y conda-build
```

```bash
conda build . --channel https://roq-trading.com/conda/stable
```

or


```bash
conda build . --channel https://roq-trading.com/conda/unstable
```

You can now find packages in the relevant directory under `$CONDA_PREFIX/conda-bld/`
