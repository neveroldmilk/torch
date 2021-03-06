% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/nnf-pooling.R
\name{nnf_adaptive_avg_pool1d}
\alias{nnf_adaptive_avg_pool1d}
\title{Adaptive_avg_pool1d}
\usage{
nnf_adaptive_avg_pool1d(input, output_size)
}
\arguments{
\item{input}{input tensor of shape (minibatch , in_channels , iW)}

\item{output_size}{the target output size (single integer)}
}
\description{
Applies a 1D adaptive average pooling over an input signal composed of
several input planes.
}
