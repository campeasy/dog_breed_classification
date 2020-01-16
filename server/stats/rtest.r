require(ggplot2)
options(stringsAsFactors = FALSE)

# Import the data and look at the first six rows
datastats <- read.csv(file = 'stats.csv')

datastats$Date <- as.Date(as.POSIXct(datastats$Timestamp, origin="1970-01-01"))

## Tabulate
date_tab <- table(datastats$Date)

date_frequency = data.frame(Date=names(date_tab), Frequency=as.vector(date_tab))

ggplot(data = date_frequency, aes(x = Date, y = Frequency)) +
      geom_bar(stat = "identity", fill = "purple") +
      labs(title = "Total daily requests",
           #subtitle = "Fall 2020",
           x = "Date", y = "Daily Requests")

class_tab <- table(datastats$Class)
class_frequency = data.frame(Names=names(class_tab), Frequency=as.vector(class_tab))

slices <- as.vector(class_frequency$Frequency)
lbls <- as.list(class_frequency$Names)
pct <- round(slices/sum(slices)*100)
lbls <- paste(substr(lbls, 1, 16), pct) # add percents to labels
lbls <- paste(lbls,"%",sep="") # ad % to labels
pie(slices,labels = lbls, col=rainbow(length(lbls)), main="Classified Classes")


sat_tab <- table(datastats$Satisfaction)
sat_frequency = data.frame(Names=names(sat_tab), Frequency=as.vector(sat_tab))
slices <- as.vector(sat_frequency$Frequency)
lbls <- as.list(sat_frequency$Names)
pct <- round(slices/sum(slices)*100)
lbls <- paste(substr(lbls, 1, 16), pct) # add percents to labels
lbls <- paste(lbls,"%",sep="") # ad % to labels
pie(slices,labels = lbls, col=rainbow(length(lbls)), main="Satisfaction")
#head(datastats)