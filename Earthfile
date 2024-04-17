VERSION 0.7


COPY_CI_DATA:
    COMMAND
    COPY --dir "ci/" ".github/" "./"


check-github-actions-workflows-linting:
    FROM golang:1.20.13
    RUN go install github.com/rhysd/actionlint/cmd/actionlint@v1.6.26
    DO +COPY_CI_DATA
    RUN ./ci/check-github-actions-workflows-linting.sh
