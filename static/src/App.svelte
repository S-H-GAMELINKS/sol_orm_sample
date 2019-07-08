<script>
    import axios from 'axios';

    let content = "";

    let createPost = () => {
        var params = new URLSearchParams();

        params.append('content', content);

        axios.post("/api/posts", params).then((response) => {
            alert(response);
        }, (error) => {
            alert(error);
        })
    }

    let getPosts = () => {
        posts.length = 0
        axios.get("/api/posts").then((response) => {
            for(let i = 0; i < response.data.length; i++) {
                alert(response.data[i])
            }
        })
    }
</script>

<input bind:value={content}>

<button on:click={createPost}>Post</button>

<button on:click={getPosts}>Get</button>
