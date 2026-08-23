import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from sklearn import datasets
from sklearn.model_selection import GridSearchCV, train_test_split
from sklearn import metrics
import random
import time
import pandas as pd 
from sklearn.tree import DecisionTreeClassifier
#ZAD1
X,y =datasets.make_classification(
    n_samples=200,
    n_features=2,
    n_informative=2,
    n_redundant=0,
    n_repeated=0,
    random_state=5,
   
    ) 

param_grid = {
    "criterion": ["gini", "entropy"],
    "max_depth": [None, 5, 10, 15, 20],
    "min_samples_split": [2, 5, 10],
    "min_samples_leaf": [1, 2, 4],
    "max_features": [None,"sqrt", "log2"]
}

clas=DecisionTreeClassifier(random_state=42)

grid_search=GridSearchCV(
    estimator=clas,
    param_grid=param_grid,
    scoring='accuracy',
    cv=5,
    n_jobs=1
    )

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

grid_search.fit(X_train, y_train) 

print("Najlepsze parametry:", grid_search.best_params_)
print("Najlepszy wynik accuracy:", grid_search.best_score_)

best_clas = grid_search.best_estimator_
test_accuracy = best_clas.score(X_test, y_test)
print("Accuracy na zbiorze testowym:", test_accuracy) 

print("")


for name,value in param_grid.items():
    single_param_grid={name:value}
    grid_search=GridSearchCV(
    estimator=clas,
    param_grid=single_param_grid,
    scoring='accuracy',
    cv=5,
    n_jobs=1
    )
    grid_search.fit(X_train, y_train)
    print("Testuje dla parametru: ",name)
    print("Najlepszy wynik accuracy:", grid_search.best_score_)
    best_clas = grid_search.best_estimator_
    test_accuracy = best_clas.score(X_test, y_test)
    print("Accuracy na zbiorze testowym:", test_accuracy) 



param_grid = {
    "min_samples_split": np.arange(1, 18, 1),
    "max_depth": np.arange(1, 18, 1)
} 
grid_search = GridSearchCV(
    estimator=clas, 
    param_grid=param_grid,
    scoring="accuracy",
    cv=5,
    n_jobs=1,
    return_train_score=True
)
grid_search.fit(X_train, y_train)
results = pd.DataFrame(grid_search.cv_results_)
mean= results.groupby('param_max_depth')['mean_test_score'].mean()


plt.plot(mean.index, mean.values * 100)
plt.xlabel("min_samples_split")
plt.ylabel("Accuracy (%)")
plt.title("Accuracy")
plt.legend()
plt.grid(True)
plt.show()
import seaborn as sns
heatmap_data = results.pivot_table(
    index='param_max_depth', 
    columns='param_min_samples_split', 
    values='mean_test_score'
)

ax = sns.heatmap(heatmap_data, annot=False,cmap="YlGnBu", square=True,            
    cbar_kws={'label': 'Accuracy', 'shrink': 0.8})

plt.gca().invert_yaxis()
plt.yticks(ticks=np.arange(0, 17), labels=np.arange(0, 17))
plt.title("Accuracy Heatmap")
plt.xlabel("min_samples_split")
plt.ylabel("max_depth")
plt.show() 

#ZADANIEK OLEJNE
accuracy = np.zeros(100)
recall = np.zeros(100)
precision = np.zeros(100)
f1 = np.zeros(100)
roc = np.zeros(100)
train_time=np.ones(100)
test_time=np.ones(100)
clas = DecisionTreeClassifier(
    criterion='entropy',
    max_depth=5,
    max_features=None,
    min_samples_leaf=2,
    min_samples_split=2
)


for i in range(100):
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42 + i)
    
    start=time.time()
    clas.fit(X_train, y_train)
    stop=time.time()
    train_time[i]=stop-start
    start=time.time()
    y_pred = clas.predict(X_test)
    stop=time.time()
    test_time[i]=stop-start

    accuracy[i] = metrics.accuracy_score(y_test, y_pred)
    recall[i] = metrics.recall_score(y_test, y_pred)
    precision[i] = metrics.precision_score(y_test, y_pred)
    f1[i] = metrics.f1_score(y_test, y_pred)
    roc[i] = metrics.roc_auc_score(y_test, y_pred)
    if i==99:

        y_scores = clas.predict_proba(X_test)[:,1] 
        fpr,tpr,threshold = metrics.roc_curve(y_test,y_scores)
        roc_auc=metrics.auc(fpr,tpr)

        plt.plot(fpr,tpr,color='navy',label=f"Krzywa ROC")
        plt.plot([0,1],[0,1],color="red",linestyle="--")
        plt.xlabel('False Positive Rate')
        plt.ylabel('True Positive Rate')
        plt.title(f'Krzywa ROC')
        plt.legend(loc="lower right")
        plt.grid(True)
        plt.show()

        h = 0.2
        x_min, x_max = X_test[:, 0].min() - 1, X_test[:, 0].max() + 1
        y_min, y_max = X_test[:, 1].min() - 1, X_test[:, 1].max() + 1
        xx, yy = np.meshgrid(np.arange(x_min, x_max, h),np.arange(y_min, y_max, h))

        Z = np.zeros(xx.shape)
        for x in range(xx.shape[0]):
            for y in range(xx.shape[1]):
                Z[x, y] = clas.predict([[xx[x, y], yy[x, y]]])[0]

        plt.contourf(xx, yy, Z, alpha=0.5, cmap=plt.cm.coolwarm)
        
        plt.contour(xx, yy, Z, levels=[0.5], colors='black', linewidths=1)
        plt.scatter(X_test[:, 0], X_test[:, 1], c=y_test, cmap=plt.cm.coolwarm, edgecolors='k')

        plt.xlabel('Cecha 1')
        plt.ylabel('Cecha 2')
        plt.title(f'Granica decyzyjna')
        plt.grid(True)
        plt.show()


metrics_names = ['Accuracy', 'Recall', 'Precision', 'F1', 'ROC AUC','Train time', 'Test time']
testy = [accuracy, recall, precision, f1, roc,train_time,test_time]
mean=np.zeros(len(testy))
for i in range(7):
    mean[i] = np.mean(testy[i])

df = pd.DataFrame(mean, index=metrics_names, columns=["Średnia"])
print(df)
