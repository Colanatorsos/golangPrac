package main

import (
	"github.com/gin-gonic/gin"
	"net/http"
)

func main() {
	r := gin.Default()

	r.GET("/hello", func(c *gin.Context) {
		c.JSON(http.StatusOK, gin.H{
			"message": "Привет, мир!",
		})
	})

	r.POST("/add", func(c *gin.Context) {
		var requestBody struct {
			Num1 int `json:"num1"`
			Num2 int `json:"num2"`
		}

		if err := c.ShouldBindJSON(&requestBody); err != nil {
			c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
			return
		}

		sum := requestBody.Num1 + requestBody.Num2
		c.JSON(http.StatusOK, gin.H{"result": sum})
	})

	r.Run(":8080")
}
