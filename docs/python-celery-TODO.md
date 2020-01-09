# Monitoring and Management Guide[¶](https://docs.celeryproject.org/en/latest/userguide/monitoring.html#monitoring-and-management-guide)



修改内容：

[how to get the queue in which a task was run - celery](https://stackoverflow.com/questions/22385297/how-to-get-the-queue-in-which-a-task-was-run-celery)





[Retrieve list of tasks in a queue in Celery](https://stackoverflow.com/questions/5544629/retrieve-list-of-tasks-in-a-queue-in-celery)



# How to send periodic tasks to specific queue in Celery

https://stackoverflow.com/questions/16962449/how-to-send-periodic-tasks-to-specific-queue-in-celery


https://stackoverflow.com/questions/29306337/how-to-stop-celery-worker-process

https://serverfault.com/questions/695787/is-there-a-more-convenient-way-to-stop-or-restart-a-detached-celery-beat-proce


# celery multiple queues



有几个模型就需要构建几个application/worker，每个application/worker根据`model_name`，`model_sub_name`就可以确定application/workername，application/workerqueue name；

显然我的所有的worker的模式完全相同，所不同的是这些worker所运行的machine learning model不同；那么我要如何来快速地为每个model生成一个worker呢？

我目前的思路是：生成各个application的代码，然后逐个启动；

第二种思路是：

```c
for model_name, model_sub_name in models:
	app = create_app(model_name, model_sub_name)
        
def create_app(model_name, model_sub_name):
	app = Celery(application_name,
             broker='redis://{}/0'.format(config.REDIS_URL),
             backend='redis://{}/1'.format(config.REDIS_URL),
             include=['proj_HS300_1_0.task_predict']
             )
    app.conf['model_name'] = model_name
    app.conf['model_sub_name'] = model_sub_name
    app.conf['queue_name'] = RedisCeleryQueueKeys.get_celery_app_queue_key(model_name, model_sub_name)
	
```





如何来启动各个worker？

`AIServerCtr.py`

```python
from ml_model import MLModelBuilder
import config
ml_model_builder = MLModelBuilder(config.ML_MODEL_DIR)
ml_model_builder.restore_ml_model(load=False)
```



如何将`model_name`, `model_sub_name`传递到各个子进程中？目前想到了如下的方法：

- 通过Environment Variables
- 通过Command-Line Arguments



经过考虑，最终的思路如下：

celery -A proj_HS300_1_0 worker -l info -c 1 -Q CeleryQueue:HS300_1:0



# [DuplicateNodenameWarning with no obvious reason](https://github.com/celery/celery/issues/2938)



<https://docs.celeryproject.org/en/latest/userguide/workers.html>



<https://docs.celeryproject.org/en/latest/faq.html>


