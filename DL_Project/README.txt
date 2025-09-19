''' Sensor-Based Human Activity Classification Challenge '''

Requirements: (Libraries used)

		1. Core Libraries:
		- pandas
		- numpy
		- seaborn
		- matplotlib

		2. Scikit-learn:
		- sklearn.preprocessing (StandardScaler, MinMaxScaler, LabelEncoder)
		- sklearn.model_selection (StratifiedKFold)
		- sklearn.metrics (classification_report, f1_score, roc_curve, auc, roc_auc_score, accuracy_score)
		- sklearn.utils.class_weight (compute_class_weight)

		3. TensorFlow/Keras:
		- tensorflow.keras.models (Model, load_model)
		- tensorflow.keras.layers (Input, Dense, LSTM, GRU, Conv1D, 
		                          Concatenate, BatchNormalization, 
		                          Dropout, Flatten, GlobalAveragePooling1D)
		- tensorflow.keras.optimizers (Adam)
		- tensorflow.keras.callbacks (EarlyStopping)
		- tensorflow.keras.utils (to_categorical)

		4. Utility:
		- timeit


Installation:
All dependencies can be installed via pip:
pip install pandas numpy seaborn matplotlib scikit-learn tensorflow


Input Training Data : pirvision_office_dataset1.csv (in the same folder as main_code.ipynb file)


Test Data file path : (e.g: pirvision_office_dataset2.csv) send as parameter to evaluate_model function in the last tab


Option_1 (Main model) checkpoints : (In checkpoints Folder)
		team_36_fold1_model.h5
		team_36_fold2_model.h5
		team_36_fold3_model.h5
		team_36_fold4_model.h5
		team_36_fold5_model.h5


Option_2 checkpoints (another variant) checkpoints : (In checkpoints Folder)
		fold1_model_opt2.h5
		fold2_model_opt2.h5
		fold3_model_opt2.h5
		fold4_model_opt2.h5
		fold5_model_opt2.h5


