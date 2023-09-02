package main

import (
	"fmt"
	"strconv"
)

func main() {
	var num1, num2 float64
	var operator string

	fmt.Print("Введите первое число: ")
	fmt.Scanln(&num1)

	fmt.Print("Введите оператор (+, -, *, /): ")
	fmt.Scanln(&operator)

	fmt.Print("Введите второе число: ")
	fmt.Scanln(&num2)

	result := calculate(num1, num2, operator)
	fmt.Printf("Результат: %.2f\n", result)
}

func calculate(num1, num2 float64, operator string) float64 {
	var result float64
	switch operator {
	case "+":
		result = num1 + num2
	case "-":
		result = num1 - num2
	case "*":
		result = num1 * num2
	case "/":
		if num2 != 0 {
			result = num1 / num2
		} else {
			fmt.Println("Ошибка: деление на ноль")
			return 0
		}
	default:
		fmt.Println("Ошибка: неверный оператор")
		return 0
	}
	return result
}
