library(tidyverse)
datatable <- read.table("Trajectories.dsv",sep='|',header=TRUE)

conditions<-as.factor(datatable$color)
ggplot(data =datatable,aes(x=aes_x,y=value,group=aes_y, color = conditions))+
  geom_line()+
  labs(title = "n=10000", group= "conditions")+
  ylab("value")+
  xlab("time")


W1 <- subset(datatable,aes_x==0.01)
W2 <- subset(datatable,aes_x==0.02)
color<- W1$color
W1<-W1$value
W2<-W2$value
conditions<-as.factor(color)
df<-data.frame(W1,W2,color)
ggplot(data=df, aes(x = W1, y = W2, color = conditions))+
  geom_point()
