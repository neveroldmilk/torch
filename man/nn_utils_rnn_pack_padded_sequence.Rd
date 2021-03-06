% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/nn-utils-rnn.R
\name{nn_utils_rnn_pack_padded_sequence}
\alias{nn_utils_rnn_pack_padded_sequence}
\title{Packs a Tensor containing padded sequences of variable length.}
\usage{
nn_utils_rnn_pack_padded_sequence(
  input,
  lengths,
  batch_first = FALSE,
  enforce_sorted = TRUE
)
}
\arguments{
\item{input}{(Tensor): padded batch of variable length sequences.}

\item{lengths}{(Tensor): list of sequences lengths of each batch element.}

\item{batch_first}{(bool, optional): if \code{TRUE}, the input is expected in \verb{B x T x *}
format.}

\item{enforce_sorted}{(bool, optional): if \code{TRUE}, the input is expected to
contain sequences sorted by length in a decreasing order. If
\code{FALSE}, the input will get sorted unconditionally. Default: \code{TRUE}.}
}
\value{
a \code{PackedSequence} object
}
\description{
\code{input} can be of size \verb{T x B x *} where \code{T} is the length of the
longest sequence (equal to \code{lengths[1]}), \code{B} is the batch size, and
\code{*} is any number of dimensions (including 0). If \code{batch_first} is
\code{TRUE}, \verb{B x T x *} \code{input} is expected.
}
\details{
For unsorted sequences, use \code{enforce_sorted = FALSE}. If \code{enforce_sorted} is
\code{TRUE}, the sequences should be sorted by length in a decreasing order, i.e.
\code{input[,1]} should be the longest sequence, and \code{input[,B]} the shortest
one. \code{enforce_sorted = TRUE} is only necessary for ONNX export.
}
\note{
This function accepts any input that has at least two dimensions. You
can apply it to pack the labels, and use the output of the RNN with
them to compute the loss directly. A Tensor can be retrieved from
a \code{PackedSequence} object by accessing its \code{.data} attribute.
}
