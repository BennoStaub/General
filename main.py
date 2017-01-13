# Import some useful libraries
import numpy as np
from sklearn.svm import SVC
from sklearn.svm import NuSVC
from sklearn.svm import LinearSVC
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import GridSearchCV
from sklearn import preprocessing


# Import user files
import src.readData as rd
import src.featureExtraction as fe
import csv


def output_list(masterList):
	output = []
	for item in masterList:
		if isinstance(item, list):
			for i in output_list(
					item):
				output.append(i)
		else:
			output.append(item)
	return output


def main():
	#------------------------------------------------------------------------
	# READ DATA
	# ------------------------------------------------------------------------

	# Read train data from file
	img_train_data = rd.read_image_data("data/set_train/")
	if img_train_data == -1:
		print("ERROR: Could not read data!")
		return


	# Read associated target values from file
	train_targets1 = rd.read_file("data/targets.csv")
	if train_targets1.all() == -1:
		print("ERROR: Could not read targets!")
		return

	# Read train data from file
	img_test_data = rd.read_image_data("data/set_test/")
	if img_test_data == -1:
		print("ERROR: Could not read data!")
		return



	# ------------------------------------------------------------------------
	# PREPROCESS AND FEATURE EXTRACTION
	# ------------------------------------------------------------------------

	len_ = len(img_test_data)
	prediction_test_targets = np.zeros((len_, 3))



	for i in xrange(0, 3):
		train_targets = train_targets1[:,i]
		# Extract features
		train_features = fe.slice_histogram(img_train_data, 25, 100)
		#train_features = fe.subvoxels_heuristics(
		#	img_train_data,
		#	subvoxels_size_=np.array([5,5,5]),
		#	print_info_="train ")


		# Scale features to zero mean and unit variance
		train_scaler = preprocessing.StandardScaler().fit(train_features)
		train_features_scaled = train_scaler.transform(train_features)




		# ------------------------------------------------------------------------
		# CLASSIFICATION MODELS
		# ------------------------------------------------------------------------

		# Declare classifier list
		classifier = [
			SVC()]


		# Make parameter lists
		C = np.arange(-1.0,4.0)
		C = 10 ** C
		gamma = np.arange(-3.0, 4.0)
		gamma = 10 ** gamma
		kernel = ["rbf", "linear"]
		coef0 = np.arange(0.0,1.0)
		degree = np.arange(3,7)


		# Generate dicts
		param_SVC = {
			"C":C,
			"kernel":kernel,
			"degree":degree,
			"gamma":gamma,
			"coef0":coef0,
			"probability":[True],
			"class_weight":["balanced"]}


		# Save dicts in list
		param_grid = [
			param_SVC]




		# ------------------------------------------------------------------------
		# CROSSVALIDATION
		# ------------------------------------------------------------------------

		print("\nCross validation")


		# Validate each classification model on subsets
		clf = []

		for cl in classifier:
			index = classifier.index(cl)

			clf.append(GridSearchCV(cl,
				param_grid[index],
				scoring="neg_log_loss",
				cv=10))

			clf[index].fit(train_features_scaled, train_targets)




		# ------------------------------------------------------------------------
		# MODEL SELECTION
		# ------------------------------------------------------------------------

		print("\nModel selection")

		# Select model according to highest score (lowest error)
		best_params = []
		best_score = []
		best_estimator = []

		for cl in clf:
			index = clf.index(cl)

			best_score.append(cl.best_score_)
			best_params.append(cl.best_params_)
			best_estimator.append(cl.best_estimator_)

		best_model_index = best_score.index(max(best_score))
		print(best_params)
		print(best_score)




		# ------------------------------------------------------------------------
		# CLASSIFICATION
		# ------------------------------------------------------------------------




		# Get features
		test_features = fe.slice_histogram(img_test_data, 25, 100)
		#test_features = fe.subvoxels_heuristics(
		#	img_test_data,
		#	subvoxels_size_=np.array([5,5,5]),
		#	print_info_="test ")


		# Preprocess features
		train_scaler = preprocessing.StandardScaler().fit(test_features)
		test_features_scaled = train_scaler.transform(test_features)


		# Predict with best model parameters
		prediction_test_targets[:,i] = best_estimator[best_model_index].predict(test_features_scaled)






	prediction_test_targets = np.where(prediction_test_targets == 1, True, False)
	shape = prediction_test_targets.shape

	submission_array = []
	list = ["ID","Sample","Label","Predicted"]
	submission_array.append(list)

	l = 0
	for i in xrange(0, shape[0]):
		list  = [l,i,"gender",prediction_test_targets[i, 0]]
		submission_array.append(list)
		l = l + 1
		list = [l,i,"age",prediction_test_targets[i, 1]]
		submission_array.append(list)
		l = l + 1
		list = [l,i,"health",prediction_test_targets[i, 2]]
		submission_array.append(list)
		l = l + 1



	with open("final_sub.csv", "wb") as f:
		writer = csv.writer(f, delimiter=',')
		for i in submission_array:
			writer.writerow(output_list(i))






	return
