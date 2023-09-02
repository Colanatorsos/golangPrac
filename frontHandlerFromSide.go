package main

import (
	"github.com/gin-gonic/gin"
	"net/http"
)

type RequestData struct {
	Action string `json:"action"`
	Data   string `json:"data"`
}

func main() {
	r := gin.Default()

	r.POST("/api/frontend", func(c *gin.Context) {
		var requestData RequestData
		if err := c.ShouldBindJSON(&requestData); err != nil {
			c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
			return
		}

		var responseData interface{}
		switch requestData.Action {
		case "action1":
			responseData = performComplexAction(requestData.Data, 1)
		case "action2":
			responseData = performComplexAction(requestData.Data, 2)
		case "action3":
			responseData = performAnotherAction(requestData.Data)
		default:
			c.JSON(http.StatusBadRequest, gin.H{"error": "Недопустимое действие"})
			return
		}

		c.JSON(http.StatusOK, gin.H{"result": responseData})
	})

	r.Run(":8080")
}

func performComplexAction(data string, num int) string {
	result := ""
	for i := 0; i < num; i++ {
		result += data
	}
	return result
}

func performAnotherAction(data string) string {
	// Можно добавить более сложную логику обработки данных
	return "Выполнено другое действие с данными: " + data
}
