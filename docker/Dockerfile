FROM continuumio/miniconda3

MAINTAINER Hans Erik Thrane <info@roq-trading.com>

ADD VERSION /version

RUN \
  conda update --quiet --name base conda && \
  conda config --add channels https://roq-trading.com/conda/unstable && \
  conda install --quiet roq-samples=$(</version)

ENTRYPOINT [ "/opt/conda/bin/example-collector" ]

CMD [ "--help" ]
