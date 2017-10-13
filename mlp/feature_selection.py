from sklearn.base import BaseEstimator, TransformerMixin
from sklearn.utils import check_random_state
from sklearn.utils.validation import check_array, check_is_fitted
from sklearn.utils.random import sample_without_replacement
import numpy as np
import math


class RandomSelection(BaseEstimator, TransformerMixin):
    """Random Selection of features"""
    def __init__(self, n_components=1000, random_state=None):
        self.n_components = n_components
        self.random_state = random_state
        self.components = None

    def fit(self, X, y=None):
        X = check_array(X)
        n_samples, n_features = X.shape

        random_state = check_random_state(self.random_state)
        self.components = sample_without_replacement(
                            n_features,
                            self.n_components,
                            random_state=random_state)
        return self

    def transform(self, X, y=None):
        check_is_fitted(self, ["components"])
        X = check_array(X)
        n_samples, n_features = X.shape
        X_new = X[:, self.components]

        return X_new


class SliceHistograms(BaseEstimator,  TransformerMixin):
    def __init__(self, n_slices=50):
        self.n_slices = n_slices

    def fit(self, X, y=None):
        return self

    def transform(self, X,  y=None):
        X = check_array(X)
        n_samples,  n_features_old = X.shape
        X = X.reshape(-1, 176, 208, 176)
        n_features_new = 45*self.n_slices
        X_new = np.zeros((n_samples, 3*n_features_new+1))
        for i in range(0, n_samples):
            feature_vector = np.array([1])
            for j in range(1, self.n_slices + 1):
                histogram_vector = np.histogram(
                    X[i, math.floor(176/self.n_slices*j-1), :, :],
                    bins=[
                        1, 100, 200, 300,  400,  500,
                        600, 700, 800, 900, 1000, 1100,
                        1200, 1300, 1400, 1500, 1600,
                        1700, 1800, 1900, 2000, 2100,
                        2200,  2300,  2400,  2500,  2600,
                        2700,  2800,  2900,  3000,  3100,
                        3200,  3300,  3400,  3500,  3600,
                        3700,  3800,  3900, 4000, 4100,
                        4200,  4300,  4400,  4500])
                feature_vector = np.concatenate((
                    feature_vector, histogram_vector[0]))
            for k in range(1, self.n_slices + 1):
                histogram_vector = np.histogram(
                    X[i, :,  math.floor(208/self.n_slices*k-1), :],
                    bins=[
                        1, 100, 200, 300,  400,  500,
                        600, 700, 800, 900, 1000, 1100,
                        1200, 1300, 1400, 1500, 1600,
                        1700, 1800, 1900, 2000, 2100,
                        2200,  2300,  2400,  2500,  2600,
                        2700,  2800,  2900,  3000,  3100,
                        3200,  3300,  3400,  3500,  3600,
                        3700,  3800,  3900, 4000, 4100,
                        4200,  4300,  4400,  4500])
                feature_vector = np.concatenate((
                    feature_vector, histogram_vector[0]))
            for l in range(1, self.n_slices + 1):
                histogram_vector = np.histogram(
                    X[i, :, :,  math.floor(176/self.n_slices*l-1)],
                    bins=[
                        1, 100, 200, 300,  400,  500,
                        600, 700, 800, 900, 1000, 1100,
                        1200, 1300, 1400, 1500, 1600,
                        1700, 1800, 1900, 2000, 2100,
                        2200,  2300,  2400,  2500,  2600,
                        2700,  2800,  2900,  3000,  3100,
                        3200,  3300,  3400,  3500,  3600,
                        3700,  3800,  3900, 4000, 4100,
                        4200,  4300,  4400,  4500])
                feature_vector = np.concatenate((
                    feature_vector, histogram_vector[0]))
            print("Processed sample " + str(i+1),  flush=True)
            X_new[i] = feature_vector
        return X_new
