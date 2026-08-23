import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from sklearn import datasets
from sklearn.naive_bayes import GaussianNB
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis
from sklearn.neighbors import KNeighborsClassifier
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split
from sklearn import metrics
import random
import time
import pandas as pd
#ZAD1
X,y =datasets.make_classification(
    n_samples=500,
    n_features=2,
    n_informative=2,
    n_clusters_per_class=2,
    n_redundant=0,
    n_repeated=0,
    random_state=5,
   
    ) 
#ZAD2
plt.scatter(X[:, 0], X[:, 1], c=y, cmap=plt.cm.coolwarm,edgecolors='k')
plt.xlabel("Cecha 1")
plt.ylabel("Cecha 2")
plt.title("Zbior losowy obiektow w 2 klasach")
plt.show() 
#ZAD3 
classifiers=[GaussianNB(),QuadraticDiscriminantAnalysis(),KNeighborsClassifier(),SVC(probability=True),DecisionTreeClassifier()]
names = ['GaussianNB', 'QDA', 'KNN', 'SVC', 'DecisionTree']
accuracy=np.ones((100,5))
recall=np.ones((100,5))
precision=np.ones((100,5))
f1=np.ones((100,5))
roc=np.ones((100,5))
train_time=np.ones((100,5))
test_time=np.ones((100,5))

for i in range(100):
    X_train,X_test,y_train,y_test=train_test_split(X,y,test_size=0.3,random_state=42+i) 
    for j in range(5):
        
        clas=classifiers[j]
        start=time.time()
        clas.fit(X_train,y_train)
        stop=time.time()
        train_time[i,j]=stop-start
        
        start=time.time()
        y_pred=clas.predict(X_test)
        stop=time.time()
        test_time[i,j]=stop-start
        
        
        accuracy[i,j]=metrics.accuracy_score(y_test,y_pred)
        recall[i,j]=metrics.recall_score(y_test,y_pred)
        precision[i,j]=metrics.precision_score(y_test,y_pred)
        f1[i,j]=metrics.f1_score(y_test,y_pred)
        roc[i,j]=metrics.roc_auc_score(y_test,y_pred)
    #RYSUNEK 2
        if i==99:
            hit_x=[]
            no_hit_x=[]
            no_hit_y=[]
            hit_y=[]
            for which,y in enumerate (y_pred):
                if y==y_test[which]:
                    hit_x.append(X_test[which][0])
                    hit_y.append(X_test[which][1])
           
                else:
                    no_hit_x.append(X_test[which][0])
                    no_hit_y.append(X_test[which][1])

            fig, axs = plt.subplots(1, 3, figsize=(18, 5))
            axs[0].scatter(X_test[:, 0], X_test[:, 1], c=y_test, cmap=plt.cm.coolwarm, edgecolors='k')
            axs[0].set_title(f'Prawdziwe klasy – klasyfikator {names[j]}')
            axs[0].set_xlabel('Cecha 1')
            axs[0].set_ylabel('Cecha 2')
            axs[0].grid(True)

            axs[1].scatter(X_test[:, 0], X_test[:, 1], c=y_pred, cmap=plt.cm.coolwarm, edgecolors='k')
            axs[1].set_title(f'Przewidziane klasy – klasyfikator {names[j]}')
            axs[1].set_xlabel('Cecha 1')
            axs[1].set_ylabel('Cecha 2')
            axs[1].grid(True)

            axs[2].scatter(hit_x, hit_y, c='green', label='Poprawne', marker='o', edgecolors='k')
            axs[2].scatter(no_hit_x, no_hit_y, c='red', label='Błędne', marker='x', edgecolors='k')
            axs[2].legend()
            axs[2].set_title(f'Trafienia i błędy – klasyfikator {names[j]}')
            axs[2].set_xlabel('Cecha 1')
            axs[2].set_ylabel('Cecha 2')
            axs[2].grid(True)

            plt.tight_layout()
            plt.show()

            #ROC
            y_scores = clas.predict_proba(X_test)[:,1] 
            fpr,tpr,threshold = metrics.roc_curve(y_test,y_scores)
            roc_auc=metrics.auc(fpr,tpr)

            plt.plot(fpr,tpr,color='navy',label=f"Krzywa ROC - klasyfikator {j+1}")
            plt.plot([0,1],[0,1],color="red",linestyle="--")
            plt.xlabel('False Positive Rate')
            plt.ylabel('True Positive Rate')
            plt.title(f'Krzywa ROC – klasyfikator {names[j]}')
            plt.legend(loc="lower right")
            plt.grid(True)
            plt.show()
            #SIATKA 
            h = 0.2
            x_min, x_max = X_test[:, 0].min() - 1, X_test[:, 0].max() + 1
            y_min, y_max = X_test[:, 1].min() - 1, X_test[:, 1].max() + 1
            xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                                 np.arange(y_min, y_max, h))

            Z = np.zeros(xx.shape)
            for x in range(xx.shape[0]):
                for y in range(xx.shape[1]):
                    Z[x, y] = clas.predict([[xx[x, y], yy[x, y]]])[0]

            plt.contourf(xx, yy, Z, alpha=0.5, cmap=plt.cm.coolwarm)
        
            plt.contour(xx, yy, Z, levels=[0.5], colors='black', linewidths=1)
            plt.scatter(X_test[:, 0], X_test[:, 1], c=y_test, cmap=plt.cm.coolwarm, edgecolors='k')

            plt.xlabel('Cecha 1')
            plt.ylabel('Cecha 2')
            plt.title(f'Granica decyzyjna – klasyfikator {names[j]}')
            plt.grid(True)
            plt.show()

    
metrics_names = ['Accuracy', 'Recall', 'Precision', 'F1', 'ROC AUC', 'Train time', 'Test time']
testy=[accuracy,recall,precision,f1,roc,train_time,test_time]
mean=np.ones((5,7))

for i in range(5):
    for j in range(7):
        mean[i,j]=np.mean(testy[j][:,i])

df=pd.DataFrame(mean,index=names,columns=metrics_names)
pd.set_option('display.max_columns', None) 
print(df)

x = np.arange(len(names))  
width = 0.1

plt.figure(figsize=(12,6))

for i in range(5):
    plt.bar(x + i*width, mean[:, i], width=width, label=metrics_names[i]) 

plt.xticks(x + width*3, names)  
plt.ylabel('Wartość średnia')
plt.title('Średnie metryki i czasy dla klasyfikatorów')
plt.legend()
plt.show()



