FROM continuumio/miniconda3

MAINTAINER Hans Erik Thrane <info@roq-trading.com>

COPY pkgs/ /pkgs/

RUN conda update --quiet --yes --name base conda && conda config --add channels file:///pkgs && conda install --quiet --yes roq-samples

ENTRYPOINT [ "/opt/conda/bin/roq-samples-collector" ]

CMD [ "--help" ]
