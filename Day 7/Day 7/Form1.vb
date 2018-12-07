Public Class Form1

    Private Structure ListMember

        Public nextNode As Integer

    End Structure

    Private Structure Worker

        Public nodeWorking As Integer
        Public timeToGo As Integer

    End Structure

    Private graph(26) As List(Of ListMember)
    Private canBeStart(26) As Integer
    Private addedToSolution(26) As Integer

    Private workers(5) As Worker

    Private Function LetterToInt(character As Char) As Integer

        Dim asciiValue As Integer = Convert.ToInt32(character)
        Dim substract As Integer = Convert.ToInt32("A"c)

        Return asciiValue - substract

    End Function

    Private Function IntToLetter(letter As Integer) As Char

        Dim add As Integer = Convert.ToInt32("A"c)
        letter = letter + add

        Return Chr(letter)

    End Function

    Private Sub ParseInput()

        Dim index As Integer = 0

        While index < TextBox1.Text.Length

            If TextBox1.Text.ElementAt(index) = "S" Then

                index = index + 5
                Dim letter1 As Char = TextBox1.Text.ElementAt(index)

                index = index + 31
                Dim letter2 As Char = TextBox1.Text.ElementAt(index)

                Dim node1 As Integer = LetterToInt(letter1)
                Dim node2 As Integer = LetterToInt(letter2)

                If canBeStart(node1) = -1 Then

                    canBeStart(node1) = 1

                End If

                canBeStart(node2) = 0

                addedToSolution(node1) = 0
                addedToSolution(node2) = 0

                Dim listElement As ListMember = New ListMember
                listElement.nextNode = node1

                graph(node2).Add(listElement)

            End If

            index = index + 1

        End While

    End Sub

    Private Function TryAddValue(node As Integer) As Boolean

        For Each nextNode As ListMember In graph(node)

            If Not (addedToSolution(nextNode.nextNode)) Then

                Return False

            End If

        Next

        addedToSolution(node) = True

        Return True

    End Function

    Private Function CouldAddValueNow(node As Integer) As Boolean

        For Each nextNode As ListMember In graph(node)

            If Not (addedToSolution(nextNode.nextNode) = 1) Then

                Return False

            End If

        Next

        Return True

    End Function

    Private Function Solve1() As String

        Dim cond As Boolean = True
        Dim result As String = ""

        Dim startNode As Integer = 0

        For index = 0 To 25
            If canBeStart(index) = 1 Then
                startNode = index
            End If
        Next

        TryAddValue(startNode)
        result = result + IntToLetter(startNode)

        While cond

            cond = False

            For index = 0 To 25

                If addedToSolution(index) = False Then

                    If TryAddValue(index) Then

                        result = result + IntToLetter(index)
                        cond = True

                        Exit For

                    End If

                End If
            Next

        End While

        Return result

    End Function

    Private Function Solve2() As Integer

        Dim totalTime As Integer = 0
        Dim cond As Boolean = True
        Dim startNode As Integer = 0

        For index = 0 To 25
            If canBeStart(index) = 1 Then
                startNode = index
            End If
        Next

        For index = 0 To 4
            workers(index) = New Worker()
            workers(index).nodeWorking = -1
            workers(index).timeToGo = 0
        Next

        workers(0).nodeWorking = startNode
        workers(0).timeToGo = 60 + startNode + 1
        addedToSolution(startNode) = -2

        While cond

            cond = False

            For index = 0 To 4

                If (workers(index).timeToGo = 0) Then

                    If Not (workers(index).nodeWorking = -1) Then

                        addedToSolution(workers(index).nodeWorking) = 1

                    End If

                    For index2 = 0 To 25

                        If addedToSolution(index2) = 0 Then

                            If CouldAddValueNow(index2) Then

                                addedToSolution(index2) = -2

                                workers(index).nodeWorking = index2
                                workers(index).timeToGo = 60 + index2 + 1

                                Exit For

                            End If

                        End If

                    Next

                End If

            Next

            For index = 0 To 4

                If Not (workers(index).timeToGo = 0) Then

                    workers(index).timeToGo = workers(index).timeToGo - 1
                    cond = True

                End If

            Next

            totalTime = totalTime + 1

        End While

        Return totalTime - 1

    End Function

    Private Sub Solve()

        For index = 0 To 25
            graph(index) = New List(Of ListMember)
            canBeStart(index) = -1
            addedToSolution(index) = -1
        Next

        ParseInput()

        Dim result1 As String = Solve1()

        For index = 0 To 25
            graph(index) = New List(Of ListMember)
            canBeStart(index) = -1
            addedToSolution(index) = -1
        Next

        ParseInput()

        Dim result2 As Integer = Solve2()

        Label3.Text = result1
        Label4.Text = result2.ToString()

    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click

        Solve()

    End Sub

End Class
