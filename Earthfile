VERSION 0.7


COPY_CI_DATA:
    COMMAND
    COPY --dir "ci/" ".github/" "./"


COPY_METADATA:
    COMMAND
    DO +COPY_CI_DATA
    COPY --dir ".git/" "./"


rust-base:
    FROM rust:1.70.0
    WORKDIR "/nasm-x86-shellcode-generator"


check-clean-git-history:
    FROM +rust-base
    RUN cargo install clean_git_history --version 0.1.2 --locked
    DO +COPY_METADATA
    ARG from_reference="origin/HEAD"
    RUN ./ci/check-clean-git-history.sh --from-reference "${from_reference}"


check-conventional-commits-linting:
    FROM +rust-base
    RUN cargo install conventional_commits_linter --version 0.12.3 --locked
    DO +COPY_METADATA
    ARG from_reference="origin/HEAD"
    RUN ./ci/check-conventional-commits-linting.sh --from-reference "${from_reference}"


golang-base:
    FROM golang:1.20.13
    ENV GOPROXY=direct
    ENV CGO_ENABLED=0
    ENV GOOS=linux
    ENV GOARCH=amd64


yaml-formatting-base:
    FROM +golang-base
    RUN go install github.com/google/yamlfmt/cmd/yamlfmt@v0.10.0
    COPY ".yamlfmt" "./"
    DO +COPY_CI_DATA


check-yaml-formatting:
    FROM +yaml-formatting-base
    RUN ./ci/check-yaml-formatting.sh


fix-yaml-formatting:
    FROM +yaml-formatting-base
    RUN ./ci/fix-yaml-formatting.sh
    SAVE ARTIFACT ".github/" AS LOCAL "./"


check-github-actions-workflows-linting:
    FROM +golang-base
    RUN go install github.com/rhysd/actionlint/cmd/actionlint@v1.6.26
    DO +COPY_CI_DATA
    RUN ./ci/check-github-actions-workflows-linting.sh


COPY_SOURCECODE:
    COMMAND
    DO +COPY_CI_DATA
    COPY --dir "src/" "tests/" "./"


compile:
    FROM gcc:13.2.0
    DO +COPY_SOURCECODE
    RUN ./ci/compile.sh
