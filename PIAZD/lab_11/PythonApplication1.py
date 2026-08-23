import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_classification
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.linear_model import LogisticRegression, Perceptron
from sklearn.multiclass import OneVsOneClassifier, OneVsRestClassifier
from sklearn import metrics
from sklearn.preprocessing import label_binarize
from sklearn.metrics import roc_curve, auc
import time
from sklearn.metrics import roc_auc_score

X, y = make_classification(
    n_samples=1600,
    n_features=2,
    n_informative=2,
    n_redundant=0,
    n_repeated=0,
    n_classes=4,
    n_clusters_per_class=1,
    random_state=5
)
classifiers = [
    SVC(kernel='linear', probability=True),
    SVC(kernel='rbf', probability=True),
    LogisticRegression(),
    Perceptron()
]

names = ['SVC linear', 'SVC rbf', 'LogisticRe', 'Perceptron']
ovs = [OneVsOneClassifier, OneVsRestClassifier]
ovs_names = ['OVO', 'OVR']

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.5, random_state=42)

plt.figure(figsize=(8,6))
scatter = plt.scatter(X_test[:, 0], X_test[:, 1], c=y_test, cmap=plt.cm.tab10, edgecolor='k')
plt.legend(*scatter.legend_elements(), title="Klasy")
plt.title("Rysunek 1: Rozrzut danych testowych w 4 klasach")
plt.xlabel("Cecha 1")
plt.ylabel("Cecha 2")
plt.grid(True)
plt.show()

trained_classifiers = []
accuracy=np.ones((4,2))
recall=np.ones((4,2))
precision=np.ones((4,2))
f1=np.ones((4,2))
roc=np.ones((4,2))
train_time=np.ones((4,2))
test_time=np.ones((4,2))
for i in range(len(classifiers)):
    for j in range(len(ovs)):
        clas = ovs[j](classifiers[i])
        
        start = time.time()
        clas.fit(X_train, y_train)
        stop = time.time()
        train_time = stop - start
        
        start = time.time()
        y_pred = clas.predict(X_test)
        stop = time.time()
        test_time = stop - start
        
        accuracy[i,j] = metrics.accuracy_score(y_test, y_pred)
        recall[i,j] = metrics.recall_score(y_test, y_pred, average='macro')
        precision[i,j] = metrics.precision_score(y_test, y_pred, average='macro')
        f1[i,j] = metrics.f1_score(y_test, y_pred, average='macro')
        #RYSUNEK 1
        hit_x, hit_y, no_hit_x, no_hit_y = [], [], [], []
        for idx, y_predicted in enumerate(y_pred):
            if y_predicted == y_test[idx]:
                hit_x.append(X_test[idx][0])
                hit_y.append(X_test[idx][1])
            else:
                no_hit_x.append(X_test[idx][0])
                no_hit_y.append(X_test[idx][1])

        fig, axs = plt.subplots(1, 3, figsize=(18, 5))

        axs[0].scatter(X_test[:, 0], X_test[:, 1], c=y_test, cmap=plt.cm.coolwarm, edgecolors='k')
        axs[0].set_title(f'Prawdziwe klasy – {names[i]} + {ovs_names[j]}')
        axs[0].set_xlabel('Cecha 1')
        axs[0].set_ylabel('Cecha 2')
        axs[0].grid(True)

        axs[1].scatter(X_test[:, 0], X_test[:, 1], c=y_pred, cmap=plt.cm.coolwarm, edgecolors='k')
        axs[1].set_title(f'Przewidziane klasy – {names[i]} + {ovs_names[j]}')
        axs[1].set_xlabel('Cecha 1')
        axs[1].set_ylabel('Cecha 2')
        axs[1].grid(True)

        axs[2].scatter(hit_x, hit_y, c='green', label='Poprawne', marker='o', edgecolors='k')
        axs[2].scatter(no_hit_x, no_hit_y, c='red', label='Błędne', marker='x', edgecolors='k')
        axs[2].legend()
        axs[2].set_title(f'Trafienia i błędy – {names[i]} + {ovs_names[j]}')
        axs[2].set_xlabel('Cecha 1')
        axs[2].set_ylabel('Cecha 2')
        axs[2].grid(True)

        plt.tight_layout()
        plt.show()

        #RYSUNEK 2
        y_test_bin = label_binarize(y_test, classes=np.unique(y_test))
        n_classes = y_test_bin.shape[1]
        if hasattr(clas, "predict_proba"):
            y_score = clas.predict_proba(X_test)
        else:
            y_score = clas.decision_function(X_test)
        roc_auc_avg = roc_auc_score(y_test_bin, y_score, average='macro', multi_class='ovr')

        roc[i,j] = roc_auc_avg
        plt.figure(figsize=(8,6))
        for k in range(n_classes):
            fpr_k, tpr_k, _ = roc_curve(y_test_bin[:, k], y_score[:, k])
            roc_auc_k = auc(fpr_k, tpr_k)
            plt.plot(fpr_k, tpr_k, lw=2, label=f'Klasa {k} (AUC = {roc_auc_k:.2f})')
        plt.plot([0, 1], [0, 1], 'k--', lw=1)
        plt.xlim([0.0, 1.0])
        plt.ylim([0.0, 1.05])
        plt.xlabel('False Positive Rate')
        plt.ylabel('True Positive Rate')
        plt.title(f'Rysunek 2: Krzywe ROC - {names[i]} + {ovs_names[j]}')
        plt.legend(loc='lower right')
        plt.grid(True)
        plt.show()
        trained_classifiers.append(clas)
h = 0.02
x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
grid_points = np.c_[xx.ravel(), yy.ravel()]

for i,clas in enumerate(trained_classifiers):
    for j in range(2):
        z = clas.predict(grid_points)
        z = z.reshape(xx.shape)

        plt.figure(figsize=(6,5))
        plt.contourf(xx, yy, z, alpha=0.3, cmap=plt.cm.coolwarm)
        plt.scatter(X_test[:, 0], X_test[:, 1], c=y_test, cmap=plt.cm.coolwarm, edgecolors='k')
        plt.title(f'powierzchnia dyskryminacyjna {names[i]} + {ovs_names[j]}')
        plt.xlabel('cecha 1')
        plt.ylabel('cecha 2')
        plt.grid(True)
        plt.show()

width=0.1
x = np.arange(len(names)*2)  

metrics_names = ['Accuracy', 'Recall', 'Precision', 'F1', 'ROC AUC']
testy = [accuracy.flatten(), recall.flatten(), precision.flatten(), f1.flatten(), roc.flatten()]

x = np.arange(len(metrics_names))  

plt.figure(figsize=(14, 5))

labels = []
for clf in names:
    for method in ovs_names:
        labels.append(f"{clf} {method}")

offsets = np.linspace(-width*3.5, width*3.5, len(labels))

for i in range(len(metrics_names)):  
    for j in range(len(labels)):
        plt.bar(x[i] + offsets[j], testy[i][j], width=width,
                label=labels[j] if i == 0 else None)

plt.xticks(x, metrics_names)
plt.ylabel('Wartość metryki')
plt.title('Metryki klasyfikatorów - skupiska według metryk, słupki: klasyfikator + OVO/OVR')
plt.legend(ncol=1, bbox_to_anchor=(1.05, 1), loc='upper left')
plt.grid(axis='y')
plt.tight_layout()
plt.show()
import pandas as pd

accuracy_flat = accuracy.flatten()
recall_flat = recall.flatten()
precision_flat = precision.flatten()
f1_flat = f1.flatten()
roc_flat = roc.flatten()

df = pd.DataFrame({
    'Klasyfikator': labels,
    'Accuracy': accuracy_flat,
    'Recall': recall_flat,
    'Precision': precision_flat,
    'F1-score': f1_flat,
    'ROC AUC': roc_flat
})

print("Wyniki klasyfikatorów:")
print(df.to_string(index=False))
